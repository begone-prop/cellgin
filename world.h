#ifndef WORLD_H_
#define WORLD_H_

#include <raymath.h>
#include <stddef.h>
#include "hashmap.h"

#define SIGN(A) (((A) > 0) * 1 + !((A) > 0) * -1)
#define VSIGN(A) {.x = SIGN((A.x)), .y = SIGN((A.y))}
#define EDGE(A, B) (((A) == 0) || (A) == (B) - 1)
#define VEDGE(A, B) (EDGE((A.x), (B)) || EDGE((A.y), (B)))

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
    map_t chunks;
} board_t;

void drawGrid(board_t);
Vector2 getCellIndex(board_t, Vector2);
Vector2 getCellPosition(board_t, Vector2);
Vector2 getChunkIndex(Vector2, size_t);
Vector2 getRelativeCellIndex(Vector2, size_t);
Vector2 getAbsoluteCellIndex(Vector2, Vector2, size_t);
int getChunkNeighbours(Vector2 *, size_t, Vector2);
#endif
