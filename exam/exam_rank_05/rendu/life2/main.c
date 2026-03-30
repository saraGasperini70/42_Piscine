#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int count_neighbors(int **map, int i, int j, int height, int width) {
    int count = 0;
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            if (x == 0 && y == 0) continue ;
            int ny = i + y, nx = j + x;
            if (ny < 0 || ny >= height || nx < 0 || nx >= width) continue ;
            count += map[ny][nx];
        }
    }
    return count;
}

void free_map(int **map, int height) {
    if (map) {
        for (int y = 0; y < height; y++) {
            free(map[y]);
        }
        free(map);
    }
}

int my_strlen(char* input) {
    int count = 0;
    while(*input != '\0') {
        count++;
        input++;
    }
    return count;
}

void write_to_map(int** map, char* input, int width, int height) {
    int input_size = my_strlen(input);
    int pen_is_active = 0;
    int pen_x = 0;
    int pen_y = 0;
    for (int i = 0; i < input_size; i++) {
        switch (input[i]) {
            case('x'): { pen_is_active = !pen_is_active; break; }
            case('w'): { if (pen_y > 0) pen_y--; break; }
            case('a'): { if (pen_x > 0) pen_x--; break; }
            case('s'): { if (pen_y < height - 1) pen_y++; break; }
            case('d'): { if (pen_x < width - 1) pen_x++; break; }
            // default: { flag = 1; break; }
        }
        if (pen_is_active) {
            map[pen_y][pen_x] = 1;
        }
    }
}

typedef struct matrix {
    int* map;
    int width;
    int height;
} matrix;

int get_value(matrix map, int x, int y) {
    if (x < 0 || y < 0 || x >= map.width || y >= map.height) {
        return -1;
    }
    return map.map[y * map.width + x];
}

void set_value(matrix map, int x, int y, int value) {
    if (x < 0 || y < 0 || x >= map.width || y >= map.height) {
        return;
    }
    map.map[y * map.width + x] = value;
}

int main(int ac, char **av) {
    if (ac != 4) return (1);

    int height = atoi(av[2]), width = atoi(av[1]), iterations = atoi(av[3]), pen_is_active = 0, flag = 0, pen_y = 0, pen_x = 0;
    char buf;
    int **map = (int **)malloc(sizeof(int *) * height);
    if (!map) return (1);
    for (int i = 0; i < height; i++) {
        map[i] = calloc(width, sizeof(int));
        if (!map[i]) {free(map); return (1);}
    }
    while (read(STDIN_FILENO, &buf, 1) == 1) {
        switch (buf) {
            case('x'): { pen_is_active = !pen_is_active; break; }
            case('w'): { if (pen_y > 0) pen_y--; break; }
            case('a'): { if (pen_x > 0) pen_x--; break; }
            case('s'): { if (pen_y < height) pen_y++; break; }
            case('d'): { if (pen_x < width) pen_x++; break; }
            default: { flag = 1; break; }
        }
        if (pen_is_active && flag == 0) {
            if (pen_y >= 0 && pen_y < height && pen_x >= 0 && pen_x < width)
                map[pen_y][pen_x] = 1;
        }
    }
    // write_to_map(map, av[4], width, height);

    for (int it = 0; it < iterations; it++) {
        int **tmp = (int **)malloc(sizeof(int *) * height);
        if (!tmp) {free_map(map, height); return (1);};
        for (int y = 0; y < height; y++) {
            tmp[y] = calloc(width, sizeof(int));
            if (!tmp[y]) { free_map(map, height); free_map(tmp, y); return (1);}
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int neighbors = count_neighbors(map, y, x, height, width);
                if (neighbors == 3 || (neighbors == 2 && map[y][x] == 1)) tmp[y][x] = 1;
                else tmp[y][x] = 0;
            }
        }
        free_map(map, height);
        // free(map);
        map = tmp;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
            putchar(map[y][x] == 1 ? 'O' : ' ');
        putchar('\n');
    }
    free_map(map, height);
    return (0);
}