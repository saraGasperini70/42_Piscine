#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct bsq {
	int **map;
	char **bd;
	int height;
	int width;
	char empty;
	char obstacle;
	char full;
	int square_size;
	int square_y;
	int square_x;
} bsq;

int prerr(bsq *bsq, FILE *filein) {
	if (bsq && bsq->map) {
		for (int i = 0; i < bsq->height; i++) free(bsq->map[i]);
		free(bsq->map);
    }
	if (bsq && bsq->bd) {
		for (int i = 0; i < bsq->height; i++) free(bsq->bd[i]);
		free(bsq->bd);
	}
	if (filein != stdin) fclose(filein);
	fprintf(stderr, "Map error.\n");
	return (1);
}

int min_3(int a, int b, int c) {
	return ((a < b) ? (a < c ? a : c) : (b < c ? b : c));
}

int main(int ac, char **av) {
	FILE *filein = stdin;
	if (ac > 1) {
		filein = fopen(av[1], "r");
		if (!filein) return prerr(NULL, filein);
	}

	bsq bsq;
	bsq.height = 0, bsq.width = 0, bsq.square_size = 0,  bsq.square_x = 0, bsq.square_y = 0;
	if (fscanf(filein, "%d %c %c %c\n", &(bsq.height), &(bsq.empty), &(bsq.obstacle), &(bsq.full)) != 4) return prerr(&bsq, filein);

	bsq.map = malloc(sizeof(int *) * bsq.height);
	bsq.bd =  malloc(sizeof(char *) * bsq.height);
	if (!bsq.map || !bsq.bd) return prerr(&bsq, filein);

	ssize_t nread = 0;
	size_t len = 0;
	char *line = NULL;
	for (int y = 0; y < bsq.height; y++) {
		nread = getline(&line, &len, filein);
		if (nread == -1) return prerr(&bsq, filein);
		if (line[(int)nread - 1] == '\n') nread--;
		if (bsq.width == 0) bsq.width = (int)nread;
		if (bsq.width != (int)nread || bsq.width <= 0) return prerr(&bsq, filein);
		bsq.map[y] = calloc(bsq.width, sizeof(int));
		bsq.bd[y] = malloc(sizeof(int) * bsq.width);
		if (!bsq.map[y] || !bsq.bd[y]) return prerr(&bsq, filein);
		for (int x = 0; x < bsq.width; x++) {
			bsq.bd[y][x] = line[x];
		}
	}

    for (int y = 0; y < bsq.height; y++) {
		for (int x = 0; x < bsq.width; x++) {
			if (bsq.bd[y][x] == bsq.obstacle) bsq.map[y][x] = 0;
			else if (y == 0 || x == 0) bsq.map[y][x] = 1;
			else {
				int min = min_3(bsq.map[y - 1][x], bsq.map[y][x - 1], bsq.map[y - 1][x - 1]);
				bsq.map[y][x] = min + 1;
			}
			if (bsq.square_size < bsq.map[y][x]) {
				bsq.square_size = bsq.map[y][x];
				bsq.square_y = y - bsq.map[y][x] + 1;
				bsq.square_x = x - bsq.map[y][x] + 1;
			}
		}
	}

	for (int y = bsq.square_y; y < bsq.square_size + bsq.square_y; y++) {
		for (int x = bsq.square_x; x < bsq.square_size + bsq.square_x; x++)
			bsq.bd[y][x] = bsq.full;
	}

	for (int y = 0; y < bsq.height; y++)
		fprintf(stdout, "%s\n", bsq.bd[y]);

	for (int y = 0; y < bsq.height; y++) {
		free(bsq.map[y]);
		free(bsq.bd[y]);
	}
	free(bsq.map);
	free(bsq.bd);
}
