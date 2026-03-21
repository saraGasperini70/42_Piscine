#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//Allowed functions: atoi, read, putchar, malloc, realloc, calloc, free

int main (int ac, char **av) {
    if (ac != 4) return (1);
    int height = atoi(av[2]);
    int width = atoi(av[1]);
    int iterations = atoi(av[3]);
    int alive = 1;
    int dead = 0;
    int **map = (int **)malloc(sizeof(int *) * height);
    if (!map) return (1);
    for (int i = 0; i < height; i++) {
        map[i] = (int *)malloc(sizeof(int) * width);
        if (!map[i]) return (1);
    }
    
    char comm;
    int pen = 0;
    int flag = 0;
    int py = 0;
    int px = 0;
    while (read(STDIN_FILENO, &comm, 1) == 1) {
        switch(comm) {
            case('w'): { if (py > 0) py--; break ; }
            case('a'): { if (px > 0) px--; break ;}
            case('s'): { if (py < (height - 1)) py++; break ;}
            case('d'): { if (px < (width - 1)) px++; break ;}
            case('x'): { pen = !pen; break ;}
            default: { flag = 1; }
        }
        if (pen == 1 && flag == 0) {
            if (py >= 0 && py < height && px >= 0 && px < width)
                map[py][px] = alive;
        }
    }
    for (int gen = 0; gen < iterations; gen++) {
        int **tmp = (int **)malloc(sizeof(int *) * height);
        if (!tmp) return (1);
        for (int i = 0; i < height; i++) {
            tmp[i] = (int *)malloc(sizeof(int) * width);
            if (!tmp[i]) return (1);
            for (int j = 0; j < width; j++) {
                int neighbors = 0;
                if (map[i - 1][j] == alive || map[i + 1][j] == alive
                || map[i][j + 1] == alive || map[i][j - 1] == alive
                || map[i - 1][j - 1] == alive || map[i + 1][j + 1] == alive
                || map[i + 1][j + 1] == alive || map[i - 1][j - 1] == alive) neighbors++;
                if (neighbors < 2 || neighbors > 3) map[i][j] = dead;
                else if ((neighbors == 2 && map[i][j] == alive) || neighbors == 3) map[i][j] = alive;
                else map[i][j] = dead;
            }
        }
        free(map);
        map = temp;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) putchar(map[i][j] == 1 ? 'O' : ' ');
        putchar('\n');
    }

    for (int i = 0; i < height; i++) free(map[i]);
    free(map);

    return(0);
}