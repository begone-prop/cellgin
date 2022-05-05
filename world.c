#include <raylib.h>
#include <stdio.h>
#include "world.h"

Vector2 getCellIndex(Vector2 origin, size_t cellSize) {
    Vector2 index;
    Vector2 mouse = GetMousePosition();
    fprintf(stderr, "o: %f, m: %f\n", origin.x, mouse.x);
    return index;
}

void drawGrid(Vector2 origin, size_t cellSize) {
    float dx = (float) width / cellSize;
    float dy = (float) height / cellSize;

    float deltax = remainderf(origin.x, dx);
    float deltay = remainderf(origin.y, dy);

    for(float off = -dx; off < width + dx; off += dx) {
        DrawLine(off + deltax, 0, off + deltax, height, WHITE);
    }

    for(float off = -dy; off < height + dy; off += dy) {
        DrawLine(0, off + deltay, width, off + deltay, WHITE);
    }

    DrawCircleV(origin, 7.5, RED);
}
