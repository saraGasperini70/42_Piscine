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
    for (int i = 0; i > height - 1 ; i++) {
        // fprintf(stderr, "Freeing current matrix at index %d\n", i);
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
    char alive = '0';
    char dead = ' ';
    if (width <= 0 || height <= 0 || iterations < 0) return 1;
    char **map = alloc_map(height, width);

    int pen = 0;
    int px = 0;
    int py = 0;

    char buf;
    int flag = 0;
    while (read(STDIN_FILENO, &buf, 1) == 1) {
        // fprintf(stderr, "buf: %c\n", buf);
        switch (buf) {
            case ('w'): { if(py > 0) py--; break;}
            case ('a'): { if(px > 0) px--; break;}
            case ('s'): { if(py < (height - 1)) py++; break;}
            case ('d'): { if(px < (width - 1)) px++; break;}
            case ('x'): { pen = !pen; break;}
            default: { flag = 1; }
        }
        // fprintf(stderr, "pen: %d\n", pen);
        if (pen == 1 && flag == 0) {
            // fprintf(stderr, "py: %d, height: %d, px: %d, width: %d\n", py, height, px, width);
            if (py >= 0 && py < height && px >= 0 && px < width) {
                map[py][px] = alive;
                // fprintf(stderr, "position (%d, %d) is %s\n", py, px, map[py][px] == alive ? "alive" : "dead");
            }
        }
    }

    // fprintf(stderr, "Pen position checked. Starting simulation...\n");
    for (int iter = 0; iter < iterations; iter++) {
        // fprintf(stderr, "Allocating temporary map...\n");
        char **temp = alloc_map(height, width);
        if (temp == NULL) { fprintf(stderr, "Error in temp allocation.\n"); return 1; }
        // fprintf(stderr, "Temp map allocated\n");

        for (int my = 0; my < height; my++) {
            for (int mx = 0; mx < width; mx++) {
                int counter = 0;
                if (my - 1 >= 0 && my + 1 < height) {
                    if (map[my - 1][mx] == alive) {
                        // fprintf(stderr, "Found alive cell at position my - 1, mx: (%d, %d)\n", my - 1, mx);
                        counter++;
                    }
                    if (map[my + 1][mx] == alive) {
                        // fprintf(stderr, "Found alive cell at position my + 1, mx: (%d, %d)\n", my + 1, mx);
                        counter++;
                    }
                    if (map[my - 1][mx - 1] == alive) {
                        // fprintf(stderr, "Found alive cell at position my, mx - 1: (%d, %d)\n", my, mx - 1);
                        counter++;
                    }
                    if (map[my + 1][mx + 1] == alive)  {
                        // fprintf(stderr, "Found alive cell at position my + 1, mx + 1: (%d, %d)\n", my + 1, mx + 1);
                        counter++;
                    }

                    if (map[my][mx - 1] == alive) {
                        // fprintf(stderr, "Found alive cell at position my, mx - 1: (%d, %d)\n", my, mx - 1);
                        counter++;
                    }
                    if (map[my][mx + 1] == alive) {
                        // fprintf(stderr, "Found alive cell at position my, mx - 1: (%d, %d)\n", my, mx - 1);
                        counter++;
                    }
                    if (map[my + 1][mx - 1] == alive)  {
                        // fprintf(stderr, "Found alive cell at position my + 1, mx - 1: (%d, %d)\n", my + 1, mx - 1);
                        counter++;
                    }
                    if (map[my - 1][mx + 1] == alive) {
                        // fprintf(stderr, "Found alive cell at position my - 1, mx + 1: (%d, %d)\n", my - 1, mx + 1);
                        counter++;
                    }
                }
                if ( (counter < 2 || counter > 3) ) temp[my][mx] = dead;
                else if (counter == 3 || (counter == 2 && temp[my][mx] == alive)) temp[my][mx] = alive;
                else temp[my][mx] = dead;
                // fprintf(stderr, "------MAP------\n");
                // fprintf(stderr, "%d", temp[my][mx] == alive ? 1 : 0);
            }
            // fprintf(stderr, "\n");
        }
        free_map(map, height);
        // fprintf(stderr, "Map freed.\n");
        // alloc_map(height, width);
        // fprintf(stderr, "Map reallocated.\n");
        map = temp;
        // free_map(temp, height);
        // fprintf(stderr, "Temp freed.\n");
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) 
            putchar(map[i][j]);
        putchar('\n');
    }
    
    return 0;
}