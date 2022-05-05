#include <raylib.h>
#include <stdio.h>
#include "world.h"

Vector2 getCellIndex(Vector2 origin, Vector2 cell) {
    Vector2 index;

    Vector2 mouse = GetMousePosition();
    Vector2 delta = Vector2Subtract(mouse, origin);

    Vector2 sign = {
        .x = SIGN(delta.x),
        .y = SIGN(delta.y)
    };

    index.x = (int)(delta.x / cell.x) + (int)sign.x;
    index.y = (int)(delta.y / cell.y) + (int)sign.y;

    return index;
}

Vector2 getCell(Vector2 origin, Vector2 cell, Vector2 index) {
    Vector2 cord;
    cord.x = index.x * cell.x + origin.x - (index.x > 0) * cell.x;
    cord.y = index.y * cell.y + origin.y - (index.y > 0) * cell.y;
    return cord;
}

void drawGrid(Vector2 origin, Vector2 cell) {

    float deltax = remainderf(origin.x, cell.x);
    float deltay = remainderf(origin.y, cell.y);

    for(float off = -cell.x; off < width + cell.x; off += cell.x) {
        DrawLine(off + deltax, 0, off + deltax, height, WHITE);
    }

    for(float off = -cell.y; off < height + cell.y; off += cell.y) {
        DrawLine(0, off + deltay, width, off + deltay, WHITE);
    }

    DrawCircleV(origin, 7.5, RED);
}
