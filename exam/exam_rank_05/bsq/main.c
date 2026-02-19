#include "bsq.h"
#include <stdio>
#include <stdlib>

int parse_map(char *filename, bsq *bsq) {

    char **filein;

    if (!filein = fopen(filename, 0_RDONLY))
        return 1;

    
    for (int i = 0; filein[0][i] != '\n'; i++) {
        if (filein[0][i] == ' ') {
            char *lines_str;
            for (int l = 0; l < i; l++)
            bsq->types->lines = atoi(filein[0][])
        }
    }
    for (int i = 0; filein[i] != '\n'; i++) {
    }
}

int main(int ac, char **av) {
    if (ac != 2)
        return 1;
    else {
        struct *bsq;
        if (!parse_map(argv[1], bsq));
            return 1;
    }
}