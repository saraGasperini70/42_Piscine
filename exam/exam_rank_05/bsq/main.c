#include "bsq.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
//Allowed functions and globals: malloc, calloc, realloc, free, fopen, fclose,
//getline, fscanf, fputs, fprintf, stderr, stdout, stdin, errno


int clear_structs(struct bsq *bsq_data) {
    if (bsq_data->map != NULL) {
        if (bsq_data->map->map != NULL) {
            for (int i = 0; i < bsq_data->map->height; i++) {
                free(bsq_data->map->map[i]);
            }
            free(bsq_data->map->map);
        }
        free(bsq_data->map);
        bsq_data->map = NULL;
    }
    if (bsq_data->types != NULL) {
        free(bsq_data->types);
        bsq_data->types = NULL;
    }
    if (bsq_data->square != NULL) {
        free(bsq_data->square);
        bsq_data->square = NULL;
    }
    return 1;
}

struct bsq *data_init() {
    struct bsq *bsq_data;
    if (!(bsq_data = malloc(sizeof(struct bsq)))) {
        fprintf(stderr, "Error: failed to allocate memory for bsq_data\n");
        return NULL;
    }
    if (!(bsq_data->map = malloc(sizeof(struct map)))) {
        fprintf(stderr, "Error: failed to allocate memory for map\n");
        free(bsq_data);
        return NULL;
    }
    if (!(bsq_data->types = malloc(sizeof(struct types)))) {
        fprintf(stderr, "Error: failed to allocate memory for types\n");
        free(bsq_data->map);
        free(bsq_data);
        return NULL;
    }
    if (!(bsq_data->square = malloc(sizeof(struct square)))) {
        fprintf(stderr, "Error: failed to allocate memory for square\n");
        free(bsq_data->types);
        free(bsq_data->map);
        free(bsq_data);
        return NULL;
    }
    /* Initialize inner struct fields to avoid using uninitialized memory */
    bsq_data->map->map = NULL;
    bsq_data->map->width = 0;
    bsq_data->map->height = 0;

    bsq_data->types->lines = 0;
    bsq_data->types->empty = 0;
    bsq_data->types->obstacle = 0;
    bsq_data->types->full = 0;

    bsq_data->square->x = 0;
    bsq_data->square->y = 0;
    bsq_data->square->size = 0;

    return bsq_data;
}

int parse_elements(struct bsq *bsq_data, FILE *filein) {
    // Check if the file pointer is valid and bsq_data is properly initialized.
    if (filein == NULL || bsq_data == NULL || bsq_data->types == NULL || bsq_data->map == NULL) {
        fprintf(stderr, "Error: invalid input data: filein=%p, bsq_data=%p, types=%p, map=%p\n", filein, bsq_data, bsq_data->types, bsq_data->map);
        return 0;
    }
    // Parse the first line of the file to get the number of lines and the characters for empty, obstacle, and full.
    int result = fscanf(filein, "%d %c %c %c\n", &(bsq_data->types->lines), &(bsq_data->types->empty), &(bsq_data->types->obstacle), &(bsq_data->types->full));
    // Validate the parsed data.
    if (result != 4) {
        fprintf(stderr, "Error: failed to parse elements\n");
        return 0;
    }
    // Additional validation checks for the parsed data: number of lines should be positive, characters should be distinct and within the valid ASCII range.
    if (bsq_data->types->lines <= 0 || bsq_data->types->empty == bsq_data->types->obstacle || bsq_data->types->empty == bsq_data->types->full || bsq_data->types->obstacle == bsq_data->types->full) {
        fprintf(stderr, "Error: invalid elements\n");
        return 0;
    }

    if (bsq_data->types->empty >= 127 || bsq_data->types->empty <= 31 || bsq_data->types->obstacle >= 127 || bsq_data->types->obstacle <= 31 || bsq_data->types->full >= 127 || bsq_data->types->full <= 31) {
        fprintf(stderr, "Error: invalid character values\n");
        return 0;
    }
    return 1;
}

int min_three(int a, int b, int c) {
    int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}

int print_square(struct bsq *bsq) {
    if (bsq == NULL || bsq->map == NULL || bsq->map->map == NULL || bsq->types == NULL || bsq->square == NULL) {
        fprintf(stderr, "Error: invalid input data for print_square. Current data:\nbsq=%p, map=%p, map->map=%p, types=%p, square=%p\nmap data:\nmap->width=%d, map->height=%d\ntypes data:\nempty=%c, obstacle=%c, full=%c\nsquare data:\nx=%d, y=%d, size=%d\n", bsq, bsq->map, bsq->map->map, bsq->types, bsq->square, bsq->map->width, bsq->map->height, bsq->types->empty, bsq->types->obstacle, bsq->types->full, bsq->square->x, bsq->square->y, bsq->square->size);
        return 0;
    }
    for (int i = bsq->square->y; i < bsq->square->y + bsq->square->size; i++) {
      for (int j = bsq->square->x; j < bsq->square->x + bsq->square->size; j++) {
        if (i < bsq->map->height && j < bsq->map->width) {
          bsq->map->map[i][j] = bsq->types->full;
        }
      }
    }
    for (int i = 0; i < bsq->map->height; i++) {
      fprintf(stdout, "%s\n", bsq->map->map[i]);
    }
    return 1;
  }

int bsq_solver(struct bsq *bsq_data) {
  // Finding the biggest square in the map and updating the bsq_data->square
  // struct with the coordinates and size of the biggest square.
    int **matrix = malloc(sizeof(int *) * bsq_data->map->height);
    for (int i = 0; i < bsq_data->map->height; i++) {
        matrix[i] = malloc(sizeof(int) * bsq_data->map->width);
    }
    int result = 0;

    //for (int i = 0; i < bsq_data->map->height; i++) {
    //    fprintf(stdout, "%s\n", bsq_data->map->map[i]);
    //}

    for (int i = 0; i < bsq_data->map->height; i++) {
      for (int j = 0; j < bsq_data->map->width; j++) {
        if (bsq_data->map->map[i][j] == bsq_data->types->obstacle)
          matrix[i][j] = 0;
        else if (i == 0 || j == 0)
          matrix[i][j] = 1;
        else {
          int min = min_three(matrix[i-1][j], matrix[i-1][j-1], matrix[i][j-1]);
          matrix[i][j] = min + 1;
        }
        if (matrix[i][j] > bsq_data->square->size) {
            bsq_data->square->size = matrix[i][j];
            bsq_data->square->x = j - matrix[i][j] + 1;
            bsq_data->square->y = i - matrix[i][j] + 1;
        }
      }
    }
    result = print_square(bsq_data);
    for (int i = 0; i < bsq_data->map->height; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return result;
}

int elements_check(char empty, char obstacle, char full, char **map, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] != empty && map[i][j] != obstacle && map[i][j] != full)
                return 0;
        }
    }
    return 1;
}

char *ft_memcpy(void *dest, const void *src, size_t n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int parse_map(char *filename, struct bsq *bsq_data) {

    FILE *filein;

    if (!(filein = fopen(filename, "r"))) {
        fprintf(stderr, "Error: failed to open file %s\n", filename);
        clear_structs(bsq_data);
        return 0;
    }

    if (!parse_elements(bsq_data, filein)) {
        fprintf(stderr, "Error: failed to parse elements from file %s\n", filename);
        clear_structs(bsq_data);
        fclose(filein);
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    bsq_data->map->height = bsq_data->types->lines;
    bsq_data->map->map = malloc(bsq_data->map->height * sizeof(char *));
    if (bsq_data->map->map == NULL) {
        fprintf(stderr, "Error: failed to allocate memory for map lines\n");
        clear_structs(bsq_data);
        fclose(filein);
        return 0;
    }

    bsq_data->map->map[0] = line;

    //fprintf(stdout, "Parsed elements: lines=%d, empty='%c', obstacle='%c', full='%c'\n", bsq_data->types->lines, bsq_data->types->empty, bsq_data->types->obstacle, bsq_data->types->full);
    //fprintf(stdout, "Map dimensions: width=%d, height=%d\n", bsq_data->map->width, bsq_data->map->height);
    line = NULL;
    len = 0;
    for (int i = 0; i < bsq_data->map->height; i++) {
        nread = getline(&line, &len, filein);
        if (bsq_data->map->width == 0) {
            bsq_data->map->width = (int)nread - 1; // Exclude the newline character
        }
        if (nread == -1) {
            fprintf(stderr, "Error: failed to read line %d of map\n", i);
            fprintf(stderr, "line: %s\nread: %zu\n", line, nread);
            clear_structs(bsq_data);
            fclose(filein);
            return 0;
        }
        if (line[(int)nread - 1] == '\n')
          nread--;
        else {
            fprintf(stderr, "Error: line %d of map is too long\n", i);
            clear_structs(bsq_data);
            fclose(filein);
            return 0;
        }
        //fprintf(stdout, "Read line %d: '%s' (length %zu)\n", i, line, nread);
        if ((int)nread != bsq_data->map->width) {
            fprintf(stderr, "Error: inconsistent line width in map: expected %d, got %zu\n", bsq_data->map->width, nread);
            clear_structs(bsq_data);
            fclose(filein);
            return 0;
        }
        bsq_data->map->map[i] = malloc((bsq_data->map->width + 1) * sizeof(char));
        if (bsq_data->map->map[i] == NULL) {
            fprintf(stderr, "Error: failed to allocate memory for map line %d\n", i);
            clear_structs(bsq_data);
            fclose(filein);
            return 0;
        }
        if (!elements_check(bsq_data->types->empty, bsq_data->types->obstacle, bsq_data->types->full, &line, 1, bsq_data->map->width)) {
            fprintf(stderr, "Error: invalid characters in map line %d\n", i);
            clear_structs(bsq_data);
            fclose(filein);
            return 0;
        }
        ft_memcpy(bsq_data->map->map[i], line, bsq_data->map->width);
        bsq_data->map->map[i][bsq_data->map->width] = '\0'; // Null-terminate the string
    }

    free(line);
    int result = bsq_solver(bsq_data);
    fclose(filein);
    return result;
}

int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    else {
        struct bsq *bsq = data_init();
        if (bsq == NULL) {
            fprintf(stderr, "Error: failed to initialize bsq data\n");
            return 1;
        }
        if (!parse_map(av[1], bsq)) {
            fprintf(stderr, "Error: failed to parse map\n");
            free(bsq);
            return 1;
        }
        clear_structs(bsq);
        free(bsq);
    }
}
