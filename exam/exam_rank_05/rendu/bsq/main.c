#include <stdio.h>
#include <stdlib.h>

void free_map_matrix(char **map, int **matrix, int height) {
    if (map) {
        for (int i = 0; i < height; i++) free(map[i]);
        free(map);
    }
    if (matrix) {
        for (int i = 0; i < height; i++) free(matrix[i]);
        free(matrix);
    }
}

int prerr(char *msg, char **map, int **matrix, int height, FILE *filein) {
    fprintf(stderr, "Error: %s\n", msg);
    free_map_matrix(map, matrix, height);
    if (filein && filein != stdin) fclose(filein);
    return (1);
}

int main (int ac, char **av) {
    FILE *filein = stdin;
    if (ac > 1) {
        filein = fopen(av[1], "r");
        if (!filein) return prerr("Can't open file", NULL, NULL, 0, NULL);
    }

    int height = 0;
    char empty, obstacle, full;
    if (fscanf(filein, "%d %c %c %c\n", &height, &empty, &obstacle, &full) != 4) return prerr("Some elements not found, expected height, empty, obstacle, full", NULL, NULL, 0, filein  );
    if (height <= 0 || empty == obstacle || empty == full || obstacle == full) return prerr("Invalid elements", NULL, NULL, 0, filein);
    char **map = (char **)malloc(sizeof(char *) * height);
    if (!map) return prerr("Invalid map/matrix alloc.", NULL, NULL, 0, filein);
    int **matrix = (int **)malloc(sizeof(int *) * height);
    if (!matrix) return prerr("Invalid map/matrix alloc.", map, NULL, 0, filein);

    ssize_t nread = 0;
    char *line = NULL;
    size_t len = 0;
    int width = -1;
    for (int i = 0; i < height; i++) {
        nread = getline(&line, &len, filein);
        if (nread == -1) {
            free(line);
            return prerr("Invalid line length (1)", map, matrix, i, filein);
        }
        if (line[(int)nread - 1] == '\n') nread--;
        if (nread <= 0) {
            free(line);
            return prerr("Read error", map, matrix, i, filein);
        }
        if (width == -1) width = (int)nread;
        else if (width != (int)nread) {
            free(line);
            return prerr("Invalid line length (3)", map, matrix, i, filein);
        }
        map[i] = (char *)malloc(sizeof(char) * (width + 1));
        if (!map[i]) {
            free(line);
            return prerr("Invalid map line alloc.", map, matrix, i, filein);
        }
        for (int j = 0; j < width; j++) {
            if (line[j] != empty && line[j] != obstacle) {
                free(line);
                return prerr("Found invalid char", map, matrix, i + 1, filein);
            }
            map[i][j] = line[j];
        }
        map[i][width] = '\0';
    }
    free(line);
    int square_size = 0;
    int y = 0;
    int x = 0;
    for (int i = 0; i < height; i++) {
        matrix[i] = (int *)malloc(sizeof(int) * width);
        if (!matrix[i]) return prerr("Invalid matrix line alloc.", map, matrix, height, filein);
        for (int j = 0; j < width; j++) {
            if (map[i][j] == obstacle) matrix[i][j] = 0;
            else if (i == 0 || j == 0) matrix[i][j] = 1;
            else {
                int min = matrix[i - 1][j] < matrix[i][j - 1] ?
                (matrix[i - 1][j] < matrix[i - 1][j - 1] ? matrix[i - 1][j] : matrix[i - 1][j - 1]) :
                (matrix[i][j - 1] < matrix[i - 1][j - 1] ? matrix[i][j - 1] : matrix[i - 1][j - 1]);
                matrix[i][j] = min + 1;
            }
            if (matrix[i][j] > square_size) {
                square_size = matrix[i][j];
                y = i - matrix[i][j] + 1;
                x = j - matrix[i][j] + 1;
            }
        }
    }
    for (int i = y; i < y + square_size; i++) {
        for (int j = x; j < x + square_size; j++) {
            map[i][j] = full;
        }
    }

    for (int i = 0; i < height; i++)
        fprintf(stdout, "%s\n", map[i]);

    if (filein && filein != stdin) fclose(filein);
    free_map_matrix(map, matrix, height);
    return (0);
}
