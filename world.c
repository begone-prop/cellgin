#include <raylib.h>
#include <stdio.h>
#include "world.h"

Vector2 getChunkIndex(board_t board, Vector2 cellIndex) {
    Vector2 chunkIndex;
    Vector2 sign = VSIGN(cellIndex);
    chunkIndex.x = (int)(cellIndex.x / (board.chunkSize + 1)) + (int)sign.x;
    chunkIndex.y = (int)(cellIndex.y / (board.chunkSize + 1)) + (int)sign.y;
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

Vector2 getCell(board_t board, Vector2 index) {
    float size = (float) board.cellSize;
    Vector2 cord;
    cord.x = index.x * size + board.origin.x - (index.x > 0) * size;
    cord.y = index.y * size + board.origin.y - (index.y > 0) * size;
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
