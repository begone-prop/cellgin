#ifndef WORLD_H_
#define WORLD_H_

#include <raymath.h>
#include <stddef.h>

#define SIGN(A) (((A) > 0) * 1 + !((A) > 0) * -1)
#define VSIGN(A) {.x = SIGN((A.x)), .y = SIGN((A.y))}

static const int width = 1000;
static const int height = 1000;

static const Vector2 screenCenter = {
    .x = (float) width / 2,
    .y = (float) height / 2
};

typedef struct board_t {
    Vector2 origin;
    size_t cellSize;
    size_t chunkSize;
} board_t;

void drawGrid(board_t);
Vector2 getCellIndex(board_t, Vector2);
Vector2 getCell(board_t, Vector2);
Vector2 getChunkIndex(board_t, Vector2);
#endif
