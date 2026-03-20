#include <stdio.h>
#include <stdlib.h>

int main(int ac, char **av) {
    FILE *filein = stdin;
    if (ac != 1) {
        filein = fopen(av[1], "r");
        if (!filein) return (1);
    }
    int height = 0;
    int width = 0;
    char empty, obstacle, full;
    if (fscanf(filein, "%d %c %c %c\n", &height, &empty, &obstacle, &full) != 4) return (fprintf(stderr, "Map error (1)"));
    if (height <= 0 || empty == obstacle || empty == full || obstacle == full) return (fprintf(stderr, "Map error (2)"));
    char **map = (char **)malloc(sizeof(char *) * height);
    int **matrix = (int **)malloc(sizeof(int *) * height);
    if (!map || !matrix) return (fprintf(stderr, "Map error (3)"));

    ssize_t nread = 0;
    size_t len = 0;
    char *line = NULL;
    int square_size = 0;
    int y = 0;
    int x = 0;
    for (int i = 0; i < height; i++) {
        nread = getline(&line, &len, filein);
        if (nread == -1) return (fprintf(stderr, "Map error (4)"));
        if (line[(int)nread -1] == '\n') nread--;
        if (width == 0) width = (int)nread;
        if (width <= 0) return (fprintf(stderr, "Map error (8)"));
        if (width != (int)nread) return (fprintf(stderr, "Map error (5)"));
        map[i] = (char *)malloc(sizeof(char) * width);
        matrix[i] = (int *)malloc(sizeof(int) * width);
        if (!map[i] || !matrix[i]) return (fprintf(stderr, "Map error (6), width: %d, %s", width, line));
        for (int j = 0; j < width; j++) {
            if (line[j] != obstacle && line[j] != empty) return (fprintf(stderr, "Map error (7)"));
            map[i][j] = line[j];
        }
        map[i][width] = '\0';
    }

    for (int i = 0; i < height; i++) {
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
    for (int i = 0; i < height; i++) {
        fprintf(stdout, "%s\n", map[i]);
    }
    return (0);
}