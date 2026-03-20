#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Allowed functions: atoi, read, putchar, malloc, realloc, calloc, free

int main (int ac, char **av) {
    if (ac != 4) return (1);
    height = atoi(av[2]);
    width = atoi(av[1]);
    iterations = atoi(av[3]);
    char alive = '0';
    char dead = ' ';
    char **map = (char **)malloc(sizeof(char *) * height);
    if (!map || !matrix) return (1);
    (for int i = 0; i < height; i++) {
        map[i] = (char *)malloc(sizeof(char) * width);
        matrix[i] = (int *)malloc(sizeof(int) * width);
        if (!map[i] || !matrix[i]) return (1);
    }
    
    char comm;
    int pen = 0;
    int flag = 0;
    int py = 0;
    int px = 0;
    while (read(STDIN_FILENO, &comm) == 1) {
        switch(comm) {
            case('x'): { pen = !pen; break ;}
            case('w'): { if (pen && py > 0) py--; break ; }
            case('s'): { if (pen && py < height) py++; break ;}
            case('a'): { if (pen && px > 0) px--; break ;}
            case('d'): { if (pen && px < width) px++; break ;}
            default: { flag = 1; }
        }
        if (pen == 1 && flag == 0) {
            if (py > 0 && py <= height && px > 0 && px <= width)
                map[py][px] = alive
        }
    }
    
}