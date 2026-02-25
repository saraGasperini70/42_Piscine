#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av) {
    // fprintf(stderr, "Found args: %s, %s, %s\n", av[1], av[2], av[3]);
    if (ac != 4) return 1;
    int width, height, iterations;
    width = atoi(av[1]);
    height = atoi(av[2]);
    iterations = atoi(av[3]);
    char alive = '0';
    char dead = ' ';
    if (width <= 0 || height <= 0 || iterations < 0) return 1;
    char **map = malloc((sizeof(char *)) * height);

    for (int i = 0; i < height; i++) {
        map[i] = malloc((sizeof(char)) * width);
        if (!map[i]) return 1;
        for (int j = 0; j < width; j++) { map[i][j] = dead; }
    }

    int pen = 0;
    int px, py = 0;

    char buf;
    // fprintf(stderr, "Creating the matrix...\n");
    while ((read(STDIN_FILENO, &buf, 1)) == 1) {
        // fprintf(stderr, "buf is: %c\n", buf);
        int flag = 0;
        switch (buf) {
            case ('w'): { if(py > 0) py--; break ; }
            case ('a'): { if(px > 0) px--; break ; };
            case ('s'): { if(py < (height - 1)) py++; break ; }
            case ('d'): { if(px < (width - 1)) px++; break ; }
            case ('x'): { pen = !pen; break ; }
            default: { flag = 1; }
        }
        if (pen == 1 && flag == 0) {
            if (py >= 0 && py < height && px >= 0 && px < width)
                map[py][px] = alive;
        }
    }

    fprintf(stderr, "Filling the map...\n");
    for (int iter = 0; iter < iterations; iter++) {
        fprintf(stdout, "iteration %d\n", iter);
        char **temp = malloc((sizeof(char *)) * height);
        for (int i = 0; i < height; i++) {
            
            temp[i] = malloc((sizeof(char)) * width);
            if (!temp[i]) return 1;
            for (int j = 0; j < width; j++) {temp[i][j] = dead;}
        }

        for (int my = -1; my < height; my++) {
            for (int mx = -1; mx < width; mx++) {
                int counter = 0;
                if (map[my - 1][mx] == alive 
                    || map[my + 1][mx] == alive || map[my][mx + 1] == alive 
                    || map[my][mx - 1] == alive || map[my - 1][mx - 1] 
                    || map[my + 1][mx + 1] || map[my + 1][mx - 1] || map[my - 1][mx + 1]) {
                    counter++;
                }
                fprintf(stderr, "counter: %d", counter);
                if ( (counter < 2 || counter > 3) ) temp[my][mx] = dead;
                else if (counter == 3 || (counter == 2 && temp[my][mx] == alive)) temp[my][mx] = alive;
                else temp[my][mx] = dead;
            }
        }
        for (int i = 0; i < height; i++)
            free(map[i]);
        free(map);
        map = temp;
    }

    // for (int i = 0; i < height; i++) {
        //     free(map[i]);
        // }
        // free(map);
        
    // fprintf(stderr, "Printing the map...");
    for (int i = 0; i < height; i++) {
        // fprintf(stderr, "map[i]: %s\n", map[i]);
        for (int j = 0; j < width; j++) 
            putchar(map[i][j]);
        putchar('\n');
    }
    
    return 0;
}