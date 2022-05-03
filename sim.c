#include <stdio.h>
#include "./sim.h"

void oneState(size_t x, size_t y, int board[][y]) {
    for(size_t dx = 0; dx < x; dx++) {
        for(size_t dy = 0; dy < y; dy++) {
            board[dx][dy] = 1;
        }
    }
}

void copyState(size_t x, size_t y, const int board[][y], int board2[][y]) {
    for(size_t dx = 0; dx < x; dx++) {
        for(size_t dy = 0; dy < y; dy++) {
            board2[dx][dy] = board[dx][dy];
        }
    }
}

void printBoard(size_t x, size_t y, int board[][y]) {
    for(size_t dx = 0; dx < x; dx++) {
        for(size_t dy = 0; dy < y; dy++) {
            fprintf(stderr, "%i", board[dx][dy]);
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\033[%zuA\r", y);
}

size_t countNeighbours(size_t x, size_t y, int board[][y], int dx, int dy) {
    size_t alive = 0;

    for(int cx = -1; cx <= 1; cx++) {
        for(int cy = -1; cy <= 1; cy++) {

            if(cx == 0 && cy == 0) continue;

            int offx = cx + dx;
            int offy = cy + dy;
            if(IS_NEIGHBOUR((int) x, (int) y, offx, offy)) {
                alive += board[offx][offy];
            }
        }
    }

    return alive;
}

void zeroState(size_t x, size_t y, int board[][y]) {
    for(size_t dx = 0; dx < x; dx++) {
        for(size_t dy = 0; dy < y; dy++) {
            board[dx][dy] = 0;
        }
    }
}

int nextState(size_t x, size_t y, int board[][y], int nextState[][y]) {

    zeroState(x, y, nextState);

    for(size_t dx = 0; dx < x; dx++) {
        for(size_t dy = 0; dy < y; dy++) {

            size_t alive = countNeighbours(x, y, board, dx, dy);

            if(board[dx][dy]) {
                if(alive == 2 || alive == 3) nextState[dx][dy] = 1;
                else nextState[dx][dy] = 0;
            } else {
                if(alive == 3) nextState[dx][dy] = 1;
                else nextState[dx][dy] = 0;
            }
        }
    }

    return 1;
}
