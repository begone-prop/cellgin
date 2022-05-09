#include <raylib.h>
#include <stdio.h>
#include "world.h"

int checkEdge(board_t *board, const chunk_t *chunk) {

    Vector2 sign = VSIGN(chunk->index);

    for(int x = 0; x < board->chunkSize; x += board->chunkSize - 1) {
        for(int y = 0; y < board->chunkSize; y++) {
            if(chunk->state[x + board->chunkSize * y]) {

                int cond = (x == 0);

                if(EDGE(x, board->chunkSize)) {
                    Vector2 corn = chunk->index;
                    corn.x = (y == 0) * (chunk->index.x - 1) + (y != 0) * (chunk->index.x + 1);
                    corn.x += (corn.x == 0) * -sign.x;
                    corn.y = cond * (chunk->index.y - 1) + !cond * (chunk->index.y + 1);
                    corn.y += (corn.y == 0) * -sign.y;

                    insert(&board->chunks, corn, board->chunkSize);
                } else {
                    Vector2 newIdx = chunk->index;
                    newIdx.y = cond * (chunk->index.y - 1) + !cond * (chunk->index.y + 1);
                    newIdx.y += (newIdx.y == 0) * -sign.y;
                    insert(&board->chunks, newIdx, board->chunkSize);
                    break;
                }
            }
        }
    }

    for(int y = 0; y < board->chunkSize; y += board->chunkSize - 1) {
        for(int x = 0; x < board->chunkSize; x++) {
            if(chunk->state[x + board->chunkSize * y]) {

                int cond = (y == 0);

                if(EDGE(x, board->chunkSize)) {
                    Vector2 corn = chunk->index;
                    corn.x = (y == 0) * (chunk->index.x - 1) + (y != 0) * (chunk->index.x + 1);
                    corn.x += (corn.x == 0) * -sign.x;
                    corn.y = cond * (chunk->index.y - 1) + !cond * (chunk->index.y + 1);
                    corn.y += (corn.y == 0) * -sign.y;
                    insert(&board->chunks, corn, board->chunkSize);
                } else {
                    Vector2 newIdx = chunk->index;
                    newIdx.x = cond * (chunk->index.x - 1) + !cond * (chunk->index.x + 1);
                    newIdx.x += (newIdx.x == 0) * -sign.x;
                    insert(&board->chunks, newIdx, board->chunkSize);
                    break;
                }
            }
        }
    }

    return 1;
}

Vector2 getAbsoluteCellIndex(Vector2 chunkIdx, Vector2 cellIdx, size_t chunkSize) {
    Vector2 abso;
    Vector2 sign = VSIGN(chunkIdx);

    abso.x = (cellIdx.x + sign.x) + ((chunkIdx.x - sign.x) * chunkSize);
    if(chunkIdx.x < 0) {
        abso.x -= (int) chunkSize + sign.x;
    }

    abso.y = (cellIdx.y + sign.y) + ((chunkIdx.y - sign.y) * chunkSize);
    if(chunkIdx.y < 0) {
        abso.y -= (int) chunkSize + sign.y;
    }

    abso.y *= -1;
    return abso;
}

Vector2 getRelativeCellIndex(Vector2 cellIndex, size_t chunkSize) {
    Vector2 rel;

    rel.x = abs((int)(cellIndex.x - 1)) % chunkSize;
    rel.y = abs((int)(cellIndex.y - 1)) % chunkSize;

    if(cellIndex.x < 0) {
        rel.x = chunkSize - (abs((int)cellIndex.x) % chunkSize);
        int check = (rel.x == chunkSize);
        rel.x =  check * 0 + !check * rel.x;
    }

    if(cellIndex.y < 0) {
        rel.y = chunkSize - (abs((int)cellIndex.y) % chunkSize);
        int check = (rel.y == chunkSize);
        rel.y =  check * 0 + !check * rel.y;
    }

    return rel;
}

Vector2 getChunkIndex(Vector2 cellIndex, size_t chunkSize) {
    Vector2 chunkIndex;
    Vector2 sign = VSIGN(cellIndex);

    chunkIndex.x = (int)((cellIndex.x - sign.x) / chunkSize) + sign.x;
    chunkIndex.y = (int)((cellIndex.y - sign.y) / chunkSize) + sign.y;

    return chunkIndex;
}

Vector2 getCellIndex(board_t board, Vector2 offset) {
    Vector2 index;
    float size = (float) board.cellSize;

    Vector2 delta = Vector2Subtract(offset, board.origin);

    Vector2 sign = VSIGN(delta);

    index.x = (int)(delta.x / size) + (int)sign.x;
    index.y = (int)(delta.y / size) + (int)sign.y;
    index.y *= -1;

    return index;
}

Vector2 getCellPosition(board_t board, Vector2 index) {
    float size = (float) board.cellSize;
    Vector2 cord;
    cord.x = (int)index.x * size + board.origin.x - (index.x > 0) * size;
    cord.y = (int)index.y * size + board.origin.y - (index.y > 0) * size;
    return cord;
}

void drawGrid(board_t board) {

    float size = (float) board.cellSize;

    float deltax = remainderf(board.origin.x, size);
    float deltay = remainderf(board.origin.y, size);

    for(float off = -size; off < width + size; off += size) {
        int cond = (off + deltax) == board.origin.x;
        DrawLineEx(
                (Vector2){off + deltax, 0},
                (Vector2){off + deltax, height},
                cond * 3.0F + !cond * 1, WHITE);
    }

    for(float off = -size; off < height + size; off += size) {
        int cond = (off + deltay) == board.origin.y;
        DrawLineEx(
                (Vector2){0, off + deltay},
                (Vector2){width, off + deltay},
                cond * 3.0F + !cond * 1, WHITE);
    }

    DrawCircleV(board.origin, 7.5, RED);
}
