#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int **malloc_2d_array(int height, int width, int need_init) {
    int **array = (int **)malloc(sizeof(int *) * height);
    if (!array) return (NULL);
    for (int i = 0; i < height; i++) {
        array[i] = (int *)malloc(sizeof(int) * width);
        if (!array[i]) {
            for (int j = 0; j < i; j++) free(array[j]);
            free(array);
            return (NULL);
        }
    }
    if (need_init) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                array[i][j] = 0;
            }
        }
    }
    return (array);
}

int count_neighbors(int **map, int height, int width, int i, int j) {
    int count = 0;
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0) continue;
            int ni = i + x, nj = j + y;
            if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                count += map[ni][nj];
            }
        }
    }
    return count;
}

int free_board(int **board, int height) {
    for (int i = 0; i < height; i++) free(board[i]);
    free(board);
    return (0);
}

int print_board(int **board, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) putchar(board[i][j] ? '0' : ' ');
        putchar('\n');
    }
    return (0);
}

int main (int ac, char **av) {
    if (ac != 4) return (1);
    int height = atoi(av[2]), width = atoi(av[1]), iterations = atoi(av[3]);
    int alive = 1, dead = 0;
    int **map = malloc_2d_array(height, width, 1);

    char comm;
    int pen = 0, flag = 0, py = 0, px = 0;
    while (read(STDIN_FILENO, &comm, 1) == 1) {
        switch(comm) {
            case('w'): { if (py > 0) py--; break ; }
            case('a'): { if (px > 0) px--; break ;}
            case('s'): { if (py < (height - 1)) py++; break ;}
            case('d'): { if (px < (width - 1)) px++; break ;}
            case('x'): { pen = !pen; break ;}
            default: { flag = 1; }
        }
        if (pen == 1 && (flag == 0)) {
          if (py >= 0 && py < height && px >= 0 && px < width) {
                map[py][px] = alive;
            }
        }
    }
    for (int gen = 0; gen < iterations; gen++) {
        int **tmp = malloc_2d_array(height, width, 0);
        if (!tmp) return (1);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int neighbors = count_neighbors(map, height, width, i, j);
                if (neighbors < 2 || neighbors > 3) tmp[i][j] = dead;
                else if ((neighbors == 2 && map[i][j] == alive) || neighbors == 3) tmp[i][j] = alive;
                else tmp[i][j] = dead;
            }
        }
        free_board(map, height);
        map = tmp;
    }
    print_board(map, height, width);

    free_board(map, height);

    return(0);
}
