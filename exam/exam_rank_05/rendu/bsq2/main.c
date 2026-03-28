#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av) {
    FILE *filein = stdin;
    if (ac > 1) {
        filein = fopen(av[1], "r");
        if (!filein) return (1);
    }
    int height = 0;
    char empty, obstacle, full;
    if (fscanf(filein, "%d %c %c %c\n", &height, &empty, &obstacle, &full) != 4)
    {
        if (filein != stdin) fclose(filein);
        fprintf(stderr, "map error\n");
        return (1);
    }
    if (height <= 0 || empty == obstacle || empty == full || obstacle == full) {
        if (filein != stdin) fclose(filein);
        fprintf(stderr, "map error\n");
        return (1);
    }
    int **matrix = (int **)malloc(sizeof(int *) * height);
    if (!matrix) { 
        if (filein != stdin) fclose(filein);
        fprintf(stderr, "map error\n");
        return (1);
    }
    ssize_t nread = 0;
    size_t len = 0;
    char *buf = NULL;
    int width = 0;
    int square_size = 0, square_y = 0, square_x = 0;
    char **map = (char **)malloc(sizeof(char *) * height);
    if (!map) {
        if (filein != stdin) fclose(filein);
        fprintf(stderr, "map error\n");
        return (1);
    }
    for (int y = 0; y < height; y++) {
        nread = getline(&buf, &len, filein);
        if (nread <= 0 && nread - 1 <= 0) {
            if (filein != stdin) fclose(filein);
            fprintf(stderr, "map error\n");
            return (1);
        }
        if (buf[(int)nread - 1] == '\n') nread--;
        if (width == 0) width = (int)nread;
        if (width != (int)nread) {
            if (filein != stdin) fclose(filein);
            fprintf(stderr, "map error\n");
            return (1);
        }
        map[y] = malloc(sizeof(char) * width);
        matrix[y] = malloc(sizeof(int) * width);
        if (!matrix[y] || !map[y]) {
            if (filein != stdin) fclose(filein);
            return (1);
        }
        for (int x = 0; x < width; x++) map[y][x] = buf[x];
        for (int x = 0; x < width; x++) {
            if (x == 0 || y == 0) matrix[y][x] = 1;
            else if (map[y][x] == obstacle) matrix[y][x] = 0;
            else {
                int min = (matrix[y - 1][x] < matrix[y][x - 1]) ?
                (matrix[y - 1][x] < matrix[y - 1][x - 1] ? matrix[y - 1][x] : matrix[y - 1][x - 1]) :
                (matrix[y][x - 1] < matrix[y - 1][x - 1] ? matrix[y] [x - 1] : matrix[y - 1][x - 1]);
                matrix[y][x] = min + 1;
            }
            if (matrix[y][x] > square_size) {
                square_size = matrix[y][x];
                square_y = y - matrix[y][x] + 1;
                square_x = x - matrix[y][x] + 1;
            }
        }
    }
    for (int y = square_y; y < square_y + square_size; y++) {
        for (int x = square_x; x < square_x + square_size; x++) map[y][x] = full;
    }
    for (int y = 0; y < height; y++) fprintf(stdout, "%s\n", map[y]);
    return (0);
}