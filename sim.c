#include <stdio.h>
#include "./sim.h"
#include <string.h>

void oneState(int *state, size_t chunkSize) {
    for(size_t dx = 0; dx < chunkSize; dx++) {
        for(size_t dy = 0; dy < chunkSize; dy++) {
            state[dx + chunkSize *dy] = 1;
        }
    }
}

void zeroState(int *state, size_t chunkSize) {
    memset(state, 0, chunkSize * chunkSize * sizeof(int));
}

size_t countNeighbours(map_t hashmap, const chunk_t *chunk, size_t chunkSize, Vector2 cell) {
    size_t alive = 0;
    Vector2 sign = VSIGN(chunk->index);

    for(int cx = -1; cx <= 1; cx++) {
        for(int cy = -1; cy <= 1; cy++) {
            if(cx == 0 && cy == 0) continue;

            int offx = cx + cell.x;
            int offy = cy + cell.y;

            Vector2 newIdx = chunk->index;
            if(offx < 0) {
                fprintf(stderr, "x is on the edge\n");

                newIdx.x = (newIdx.x == 1) * -1 + !(newIdx.x == 1) * (newIdx.x - 1);
                chunk_t *new = find(hashmap, newIdx);

                if(new) {

                }

                fprintf(stderr, "Neighbour chunk: {x: %i, y: %i}\n", (int)newIdx.x, (int)newIdx.y);

            } else if(offx > chunkSize - 1) {
                fprintf(stderr, "x is on the edge\n");
            }

            if(offy < 0) {
                fprintf(stderr, "y is on the edge\n");
            } else if(offy > chunkSize - 1) {
                fprintf(stderr, "y is on the edge\n");
            }

            /*if(IS_NEIGHBOUR(chunkSize, offx, offy)) {*/
                /*alive += chunk->state[offy + chunkSize * offx];*/
            /*}*/
        }
    }

    return alive;
}


/*void copyState(size_t x, size_t y, const int board[][y], int board2[][y]) {*/
    /*for(size_t dx = 0; dx < x; dx++) {*/
        /*for(size_t dy = 0; dy < y; dy++) {*/
            /*board2[dx][dy] = board[dx][dy];*/
        /*}*/
    /*}*/
/*}*/

/*void printBoard(size_t x, size_t y, int board[][y]) {*/
    /*for(size_t dx = 0; dx < x; dx++) {*/
        /*for(size_t dy = 0; dy < y; dy++) {*/
            /*fprintf(stderr, "%i", board[dx][dy]);*/
        /*}*/
        /*fprintf(stderr, "\n");*/
    /*}*/
    /*fprintf(stderr, "\033[%zuA\r", y);*/
/*}*/


/*int nextState(size_t x, size_t y, int board[][y], int nextState[][y]) {*/

    /*[>zeroState(x, y, nextState);<]*/

    /*for(size_t dx = 0; dx < x; dx++) {*/
        /*for(size_t dy = 0; dy < y; dy++) {*/

            /*size_t alive = countNeighbours(x, y, board, dx, dy);*/

            /*if(board[dx][dy]) {*/
                /*if(alive == 2 || alive == 3) nextState[dx][dy] = 1;*/
                /*else nextState[dx][dy] = 0;*/
            /*} else {*/
                /*if(alive == 3) nextState[dx][dy] = 1;*/
                /*else nextState[dx][dy] = 0;*/
            /*}*/
        /*}*/
    /*}*/

    /*return 1;*/
/*}*/
