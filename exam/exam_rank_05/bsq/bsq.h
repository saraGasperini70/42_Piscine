#ifndef BSQ_H
# define BSQ_H

struct types {
    int lines;
    char empty;
    char obstacle;
    char full;
};

struct map {
    int width;
    int height;
};

struct bsq {
    map *map;
    types *types;
};

#endif