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

    const size_t cellSize = 25;

    Vector2 mouse, prevMouse, tmouse;

    size_t frame = 0;
    while(!WindowShouldClose()) {
        mouse = GetMousePosition();

        if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            Vector2 delta = Vector2Subtract(mouse, prevMouse);
            origin = Vector2Add(origin, delta);
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            origin = screenCenter;
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 idx = getCellIndex(origin, cellSize);
        }


        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        drawGrid(origin, cellSize);

        prevMouse = mouse;
        frame = (frame + 1) % 1000;
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
