#include <stdio.h>
#include <stdlib.h>

int prerr(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return (1);
}

int main (int ac, char **av) {
    FILE *filein = stdin;
    if (ac > 1) {
        filein = fopen(av[1], "r");
        if (!filein) return prerr("Can't open file");
    }

    int height = 0;
    char empty, obstacle, full;
    if (fscanf(filein, "%d %c %c %c\n", &height, &empty, &obstacle, &full) != 4) return prerr("Some elements not found, expected height, empty, obstacle, full");
    if (height <= 0 || empty == obstacle || empty == full || obstacle == full) return prerr("Invalid elements");
    char **map = (char **)malloc(sizeof(char *) * height);
    int **matrix = (int **)malloc(sizeof(int *) * height);
    if (!map || !matrix) return prerr("Invalid map/matrix alloc.");

    ssize_t nread = 0;
    char *line = NULL;
    size_t len = 0;
    int width = 0;
    for (int i = 0; i < height; i++) {
        nread = getline(&line, &len, filein);
        if (nread == -1) return prerr("Invalid line length (1)");
        if (line[(int)nread - 1] == '\n') nread--;
        width = (int)nread;
        if (width <= 0) return prerr("Read error");
        if (width != (int)nread) return prerr("Invalid line length (3)");
        map[i] = (char *)malloc(sizeof(char) * width);
        if (!map[i]) return prerr("Invalid map line alloc.");
        for (int j = 0; j < width; j++) {
            if (line[j] != empty && line[j] != obstacle) return prerr("Found invalid char");
            map[i][j] = line[j];
        }
        map[i][width] = '\0';
    }
    int square_size = 0;
    int y = 0;
    int x = 0;
    for (int i = 0; i < height; i++) {
        matrix[i] = (int *)malloc(sizeof(int) * width);
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

    for (int i = 0; i < height; i++) {
        free(map[i]);
        free(matrix[i]);
    }
    free(map);
    free(matrix);
    return (0);
}