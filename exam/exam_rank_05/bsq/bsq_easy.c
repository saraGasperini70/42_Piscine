#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

/*
 Minimal bsq starter per simplified spec.
 - Reads single map from file (or stdin if no arg)
 - Header: <height> <empty> <obstacle> <full>\n
 - Prints solved map only to stdout on success.
 - Prints "map error\n" to stderr on invalid input and returns non-zero.
*/

/* Free a 2D map previously allocated with malloc per row. */
static void free_map(char **map, int h) {
    if (!map) return;
    for (int i = 0; i < h; i++) {
        free(map[i]);
    }
    free(map);
}

/* Error printer */
void print_err() {
    fprintf(stderr, "map error\n");
}

int main(int ac, char **av) {
    /* If a filename is provided open it, otherwise read from stdin. */
    FILE *file = stdin;
    if (ac == 2) {
        file = fopen(av[1], "r");
        if (!file) {
            print_err();
            return 1;
        }
    }

    /* Read header: height and the three characters. */
    int height;
    char empty, obstacle, full;
    if (fscanf(file, "%d %c %c %c", &height, &empty, &obstacle, &full) != 4) {
        /* Invalid header */
        print_err();
        if (file != stdin) fclose(file);
        return 1;
    }

    /* Basic header validation to keep the simplified constraints. */
    if (height <= 0 || height > 100) {
        print_err();
        if (file != stdin) fclose(file);
        return 1;
    }
    if (empty == obstacle || empty == full || obstacle == full) {
        print_err();
        if (file != stdin) fclose(file);
        return 1;
    }

    /* Read the first map line to determine width. Use getline() return value
       (nread) as the actual number of bytes read; len is buffer capacity. */
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, file);
    if (nread == -1) {
        print_err();
        free(line);
        if (file != stdin) fclose(file);
        return 1;
    }
    if (line[nread-1] == '\n') nread--;
    int width = (int)nread;
    if (width <= 0 || width > 100) {
        print_err();
        free(line);
        if (file != stdin) fclose(file);
        return 1;
    }

    /* Allocate array of row pointers. Each row will be allocated separately. */
    char **map = malloc(sizeof(char*) * height);
    if (!map) {
        print_err();
        free(line);
        if (file != stdin) fclose(file);
        return 1;
    }

    /* Store first read line into map[0] (copy contents). */
    map[0] = malloc(width + 1);
    if (!map[0]) { free_map(map, 1); free(line); if (file!=stdin) fclose(file); print_err(); return 1; }
    for (int j = 0; j < width; j++) map[0][j] = line[j];
    map[0][width] = '\0';

    /* Read the remaining lines and copy them into allocated rows. */
    for (int i = 1; i < height; i++) {
        nread = getline(&line, &len, file);
        if (nread == -1) {
            /* Missing lines or read error */
            print_err();
            free_map(map, i); free(line); if (file!=stdin) fclose(file); return 1;
        }
        if (line[nread-1] == '\n') nread--;
        if ((int)nread != width) {
            /* Inconsistent width */
            print_err();
            free_map(map, i); free(line); if (file!=stdin) fclose(file); return 1;
        }
        map[i] = malloc(width + 1);
        if (!map[i]) { free_map(map, i); free(line); if (file!=stdin) fclose(file); print_err(); return 1; }
        for (int j = 0; j < width; j++) map[i][j] = line[j];
        map[i][width] = '\0';
    }

    /* Done reading; release getline buffer and input file. */
    free(line);
    if (file != stdin) fclose(file);

    /* Validate that only allowed characters are present (empty or obstacle). */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char c = map[i][j];
            if (c != empty && c != obstacle) {
                free_map(map, height);
                print_err();
                return 1;
            }
        }
    }

    /* Use a single flattened DP array sized height*width to compute largest
       square ending at each cell. dp[i*width + j] stores the size. */
    int *dp = malloc(sizeof(int) * height * width);
    if (!dp) { free_map(map, height); print_err(); return 1; }
    for (int i = 0; i < height * width; i++) dp[i] = 0;

    int best_size = 0;
    int best_x = 0, best_y = 0;

    /* Fill DP table. For obstacles dp=0; first row/col dp=1 for empties. */
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = i * width + j;
            if (map[i][j] == obstacle) {
                dp[idx] = 0;
            } else if (i == 0 || j == 0) {
                dp[idx] = 1;
            } else {
                int a = dp[(i-1)*width + j];
                int b = dp[(i-1)*width + (j-1)];
                int c = dp[i*width + (j-1)];
                int min = a < b ? a : b;
                min = min < c ? min : c;
                dp[idx] = min + 1;
            }
            /* Track best square size and its bottom-right coordinate. */
            if (dp[idx] > best_size) {
                best_size = dp[idx];
                best_x = j;
                best_y = i;
            }
        }
    }

    /* Replace empty chars with full char inside the best square found. */
    if (best_size > 0) {
        for (int i = best_y - best_size + 1; i <= best_y; i++) {
            for (int j = best_x - best_size + 1; j <= best_x; j++) {
                map[i][j] = full;
            }
        }
    }

    /* Print solved map exactly as required: no extra text. */
    for (int i = 0; i < height; i++) {
        fprintf(stdout, "%s\n", map[i]);
    }

    free(dp);
    free_map(map, height);
    return 0;
}
