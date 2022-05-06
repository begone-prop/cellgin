#define _DEFAULT_SOURCE

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include <fcntl.h>
#include "sim.h"
#include "world.h"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    InitWindow(width, height, "Game of Life");
    bool animate = false;
    const Vector2 screenCenter = {
        .x = (float) width / 2,
        .y = (float) height / 2
    };

    Vector2 origin = screenCenter;

    Color pal[] = {BLACK, WHITE};
    bool black = false;
    static const size_t tick = 30;

    const size_t cellDens = 15;
    const Vector2 cell = {
        .x = (int)(width / cellDens),
        .y = (int)(width / cellDens),
    };
    const size_t cellSize = width / cellDens;

    Vector2 mouse, prevMouse, tmouse;

    static int grid[10][10];
    size_t frame = 0;
    Vector2 idx, current;
    idx.x = 0;
    idx.y = 0;

    while(!WindowShouldClose()) {
        mouse = GetMousePosition();

        if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            Vector2 delta = Vector2Subtract(mouse, prevMouse);
            origin = Vector2Add(origin, delta);
            current = Vector2Add(current, delta);
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            origin = screenCenter;
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            idx = getCellIndex(origin, cellSize, mouse);
            current = getCell(origin, cellSize, idx);

            fprintf(stderr, "(x: %.0f, y: %.0f)\n", idx.x, -idx.y);
        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        /*DrawRectangleV(current, cell, RAYWHITE);*/
        drawGrid(origin, cellSize);

        prevMouse = mouse;
        if(!(frame % tick)) fprintf(stderr, "dt: %.05fs\r", GetFrameTime());
        frame = (frame + 1) % tick;
        EndDrawing();
    }

    CloseWindow();

  return 0;
}
