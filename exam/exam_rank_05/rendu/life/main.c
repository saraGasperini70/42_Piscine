#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Allowed functions: atoi, read, putchar, malloc, realloc, calloc, free

int main (int ac, char **av) {
    int height = 0;
    int width = 0;
    int iterations = 0;
    if (ac == 4) {
        height = atoi(av[2]);
        width = atoi(av[1]);
        iterations = atoi(av[3]);
    }
    else if (ac == 2) {

    }
    char *comms = NULL;
    ssize_t nread = read(stdin, comms);
    if (nread == -1) return (1);
    char **tmp;
    
}