#include <raylib.h>
#include <stdio.h>
#include "world.h"

Vector2 getCellIndex(Vector2 origin, size_t cellSize, Vector2 offset) {
    Vector2 index;
    float size = (float) cellSize;

    Vector2 delta = Vector2Subtract(offset, origin);

    Vector2 sign = {
        .x = SIGN(delta.x),
        .y = SIGN(delta.y)
    };

    index.x = (int)(delta.x / size) + (int)sign.x;
    index.y = (int)(delta.y / size) + (int)sign.y;
    index.y *= -1;

    return index;
}

Vector2 getCell(Vector2 origin, size_t cellSize, Vector2 index) {
    float size = (float) cellSize;
    Vector2 cord;
    cord.x = index.x * size + origin.x - (index.x > 0) * size;
    cord.y = index.y * size + origin.y - (index.y > 0) * size;
    return cord;
}

void drawGrid(Vector2 origin, size_t cellSize) {

    float size = (float) cellSize;

    float deltax = remainderf(origin.x, cellSize);
    float deltay = remainderf(origin.y, cellSize);

    for(float off = -size; off < width + size; off += size) {
        int cond = (off + deltax) == origin.x;
        DrawLineEx(
                (Vector2){off + deltax, 0},
                (Vector2){off + deltax, height},
                cond * 3.0F + !cond * 1, WHITE);
    }

    for(float off = -size; off < height + size; off += size) {
        int cond = (off + deltay) == origin.y;
        DrawLineEx(
                (Vector2){0, off + deltay},
                (Vector2){width, off + deltay},
                cond * 3.0F + !cond * 1, WHITE);
    }

    DrawCircleV(origin, 7.5, RED);
}
