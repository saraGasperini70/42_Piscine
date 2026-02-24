#ifndef BSQ_H
# define BSQ_H

# include <stdio.h>
#include <stdlib.h>

struct types {
    int lines;
    char empty;
    char obstacle;
    char full;
};

struct map {
    int width;
    int height;
    char **map;
};

struct square {
    int x;
    int y;
    int size;
};

struct bsq {
    struct map *map;
    struct types *types;
    struct square *square;
};

#endif
