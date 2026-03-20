#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 Simple "life" starter program (reduced, commented, and using only allowed functions).
 Usage: ./life_easy width height iterations
 Reads drawing commands from stdin (w/a/s/d to move, x to toggle pen)
 Builds initial board, simulates `iterations` generations of Conway's Game of Life,
 and prints the final board to stdout using 'O' for alive and ' ' for dead.

 Allowed functions used: atoi, read, putchar, malloc, calloc, free

 Notes / simplifications:
 - Pen starts at top-left (0,0) and is initially UP (not drawing). Toggle with 'x'.
 - Movements that would go outside the board are clamped (pen stays at edge).
 - Input is processed byte-by-byte from stdin; other characters are ignored.
 - Board size limits are not strictly enforced, but very large sizes may fail allocation.
*/

static void print_board(unsigned char *board, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            putchar(board[y * w + x] ? 'O' : ' ');
        }
        putchar('\n');
    }
}

int main(int ac, char **av) {
    if (ac != 4) {
        return 1;
    }
    int width = atoi(av[1]);
    int height = atoi(av[2]);
    int iterations = atoi(av[3]);
    if (width <= 0 || height <= 0 || iterations < 0) return 1;

    size_t size = (size_t)width * (size_t)height;
    unsigned char *board = calloc(size, 1); /* 0 = dead, 1 = alive */
    if (!board) return 1;

    /* Pen state and position */
    int pen = 0; /* 0 = up, 1 = down */
    int x = 0, y = 0; /* current pen position, top-left origin */

    /* Read commands from stdin using read(2); process on the fly. */
    char buf[4096];
    ssize_t r;
    while ((r = read(0, buf, sizeof(buf))) > 0) {
        for (ssize_t i = 0; i < r; i++) {
            char c = buf[i];
            if (c == 'x') {
                /* toggle pen and draw at current position if pen becomes down */
                pen = !pen;
                if (pen) {
                    board[y * width + x] = 1;
                }
            } else if (c == 'w') {
                if (y > 0) y--;
                if (pen) board[y * width + x] = 1;
            } else if (c == 's') {
                if (y < height - 1) y++;
                if (pen) board[y * width + x] = 1;
            } else if (c == 'a') {
                if (x > 0) x--;
                if (pen) board[y * width + x] = 1;
            } else if (c == 'd') {
                if (x < width - 1) x++;
                if (pen) board[y * width + x] = 1;
            } else {
                /* ignore other characters */
            }
        }
    }

    /* Simulate Game of Life for `iterations` generations. Use two buffers. */
    unsigned char *next = malloc(size);
    if (!next) { free(board); return 1; }

    for (int gen = 0; gen < iterations; gen++) {
        for (int yy = 0; yy < height; yy++) {
            for (int xx = 0; xx < width; xx++) {
                int count = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = xx + dx;
                        int ny = yy + dy;
                        if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue; /* outside = dead */
                        if (board[ny * width + nx]) count++;
                    }
                }
                unsigned char alive = board[yy * width + xx];
                if (alive) {
                    /* survive if 2 or 3 neighbours */
                    next[yy * width + xx] = (count == 2 || count == 3) ? 1 : 0;
                } else {
                    /* born if exactly 3 neighbours */
                    next[yy * width + xx] = (count == 3) ? 1 : 0;
                }
            }
        }
        /* swap buffers */
        unsigned char *tmp = board; board = next; next = tmp;
    }

    /* Print final board and clean up. */
    print_board(board, width, height);
    free(board);
    free(next);
    return 0;
}
