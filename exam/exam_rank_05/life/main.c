#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char **alloc_map(int height, int width) {
    char **result = (char**)malloc((sizeof(char *)) * height);

    if (!result) return NULL;

    for (int i = 0; i < height; i++) {
        result[i] = (char*)malloc((sizeof(char)) * width);
        if (!result[i]) return NULL;
        for (int j = 0; j < width; j++) { result[i][j] = ' '; }
    }

    return result;
}

void free_map(char** map, int height) {
    for (int i = 0; i < height ; i++) {
        free(map[i]);
    }
    free(map);
}

int main(int ac, char **av) {
    if (ac != 4) return 1;
    int width, height, iterations;
    width = atoi(av[1]);
    height = atoi(av[2]);
    iterations = atoi(av[3]);
    char alive = 'O';
    char dead = ' ';
    if (width <= 0 || height <= 0 || iterations < 0) return 1;
    char **map = alloc_map(height, width);

    int pen = 0;
    int px = 0;
    int py = 0;

    char buf;
    int flag = 0;
    while (read(STDIN_FILENO, &buf, 1) == 1) {
        switch (buf) {
            case ('w'): { if(py > 0) py--; break;}
            case ('a'): { if(px > 0) px--; break;}
            case ('s'): { if(py < (height - 1)) py++; break;}
            case ('d'): { if(px < (width - 1)) px++; break;}
            case ('x'): { pen = !pen; break;}
            default: { flag = 1; }
        }
        if (pen == 1 && flag == 0) {
            if (py >= 0 && py < height && px >= 0 && px < width) {
                map[py][px] = alive;
            }
        }
    }

    for (int iter = 0; iter < iterations; iter++) {
        char **temp = alloc_map(height, width);
        if (temp == NULL) { fprintf(stderr, "Error in temp allocation.\n"); return 1; }

        for (int my = 0; my < height; my++) {
            for (int mx = 0; mx < width; mx++) {
                int counter = 0;
                if (my - 1 >= 0 && my + 1 < height) {
                    int cases[3] = { -1, +1, 0};
                    for (int y = 0; y < 3; y++) {
                        for (int x = 0; x < 3; x++) {
                            if (map[cases[y]][cases[x]] == alive) counter++;
                        }
                    }
                }
                if ( (counter < 2 || counter > 3) ) temp[my][mx] = dead;
                else if (counter == 3 || (counter == 2 && temp[my][mx] == alive)) temp[my][mx] = alive;
                else temp[my][mx] = dead;
            }
        }
        free_map(map, height);
        map = temp;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) 
            putchar(map[i][j]);
        putchar('\n');
    }
    free_map(map, height);
    
    return 0;
}