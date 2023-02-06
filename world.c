#include <raylib.h>
#include <stdio.h>
#include "world.h"

static void printVector2(const char *msg, Vector2 vec) {
    const char *M = msg ? msg : "";
    fprintf(stderr, "%s: {x %f, y: %f}\n", M, vec.x, vec.y);
}

static inline int inView(Vector2 pos, size_t pad, Vector2 winSize) {
    return (pos.x >= -(long)pad) && (pos.x <= winSize.x) &&
        (pos.y >= -(long)pad) && (pos.y <= winSize.y);
}

void scaleWorld(board_t *board, Vector2 pos, float scale) {
    if(scale == 0) return;

    int old = board->cellSize;

    if(board->cellSize <= 1) {
        if(scale > 0) board->cellSize += scale;
        else board->cellSize += scale;
    } else {
        board->cellSize += scale;
    }
}

void drawCells(board_t board) {
    Color pal[] = {BLACK, WHITE};
    for(size_t idx = 0; idx < board.chunks.size; idx++) {
        if(!board.chunks.slots[idx]) continue;
        for(chunk_t *current = board.chunks.slots[idx]; current; current = current->next) {
            Vector2 rect = getChunkPosition(board, current->index);
            size_t size = board.chunkSize * board.cellSize;
            Color allocChunk = ColorAlpha(GREEN, 0.4f);
            DrawRectangleV(rect, (Vector2){size, size}, allocChunk);
            for(int i = 0; (size_t)i < board.chunkSize; i++) {
                for(int j = 0; (size_t)j < board.chunkSize; j++) {
                    int live = getCellValue_(current, (Vector2){i, j}, board.chunkSize);
                    Color prim = pal[live];
                    Color sec = pal[!live];
                    int x = (i + board.chunkSize * j) % board.chunkSize;
                    int y = (i + board.chunkSize * j) / board.chunkSize;
                    Vector2 abso = getAbsoluteCellIndex(current->index, (Vector2){x, y}, board.chunkSize);
                    Vector2 pos = getCellPosition(board, abso);

                    if(!inView(pos, board.cellSize, (Vector2){width, height}))
                        continue;

                    Rectangle rec = {
                        .x = pos.x,
                        .y = pos.y,
                        .height = board.cellSize,
                        .width = board.cellSize,
                    };

                    if(live) {
                        DrawRectangleRec(rec, prim);
                        /*DrawRectangleLinesEx(rec, 2.5f, sec);*/
                    }
                }
            }
        }
    }
}

int getChunkNeighbours(Vector2 *neigh, size_t neighSize, Vector2 index) {
    if(!neigh || neighSize != 8) return 0;

    Vector2 sign = VSIGN(index);

    size_t size = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            Vector2 newn = { index.x + i, index.y + j };
            newn.x += (newn.x == 0) * -sign.x;
            newn.y += (newn.y == 0) * -sign.y;
            neigh[size++] = newn;
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

    Vector2 delta = Vector2Subtract(offset, board.origin);
    Vector2 sign = VSIGN(delta);

    index.x = (int)(delta.x / board.cellSize) + sign.x;
    index.y = (int)(delta.y / board.cellSize) + sign.y;
    index.y *= -1;

    return index;
}

Vector2 getChunkPosition(board_t board, Vector2 index) {
    Vector2 cord;
    Vector2 sign = VSIGN(index);

    size_t size = board.chunkSize * board.cellSize;
    cord.x = index.x * size + board.origin.x - (index.x > 0) * size;
    cord.y = ((index.y * -sign.y) * size + (board.origin.y * sign.y) +
            (sign.y < 0) * size) * sign.y;

    return cord;
}

Vector2 getCellPosition(board_t board, Vector2 index) {
    float size = (float) board.cellSize;
    Vector2 cord;
    cord.x = (int)index.x * size + board.origin.x - (index.x > 0) * size;
    cord.y = (int)index.y * size + board.origin.y - (index.y > 0) * size;
    return cord;
}

void drawGrid(board_t board) {

    long size = board.cellSize;

    if((width / board.cellSize) > 75) {
        size *= board.chunkSize;
    }

    int deltax = (int)board.origin.x % (int)size;
    int deltay = (int)board.origin.y % (int)size;

    for(int off = -size; off < width + size; off += size) {
        int cond = (off + deltax) == board.origin.x;

        DrawLineEx(
                (Vector2){off + deltax, 0},
                (Vector2){off + deltax, height},
                cond * 3.0F + !cond * 1, WHITE);
    }

    for(int off = -size; off < height + size; off += size) {
        int cond = (off + deltay) == board.origin.y;
        DrawLineEx(
                (Vector2){0, off + deltay},
                (Vector2){width, off + deltay},
                cond * 3.0F + !cond * 1, WHITE);
    }

    DrawCircleV(board.origin, 7.5, RED);
}
