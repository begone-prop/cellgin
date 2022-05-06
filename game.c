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


    size_t chunkSize = 10;
    Color pal[] = {BLACK, WHITE};
    bool black = false;
    static const size_t tick = 120;

    const size_t cellDens = 15;
    const Vector2 cell = {
        .x = (int)(width / cellDens),
        .y = (int)(width / cellDens),
    };
    const size_t cellSize = width / cellDens;

    int state[chunkSize][chunkSize];
    int newState[chunkSize][chunkSize];
    zeroState(chunkSize, chunkSize, state);
    zeroState(chunkSize, chunkSize, newState);

    /*Vector2 origin = screenCenter;*/
    Vector2 origin = {
        .x = cellSize,
        .y = width - cellSize,
    };

    Vector2 mouse, prevMouse, tmouse;


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

        if(IsKeyPressed(KEY_SPACE)) {
            animate ^= 1;
        }

        if(IsKeyPressed(KEY_Z)) {
            zeroState(10, 10, state);
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            idx = getCellIndex(origin, cellSize, mouse);
            animate = false;

            fprintf(stderr, "(x: %.0f, y: %.0f)\n", idx.x, idx.y);
            if(idx.x > 0 && idx.y > 0 && idx.x <= 10 && idx.y <= 10)
                state[(int)idx.x - 1][(int)idx.y - 1] ^= 1;
        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        /*if(animate && !(frame % tick)) {*/
            /*nextState(10, 10, state, newState);*/
            /*copyState(10, 10, newState, state);*/
        /*}*/

        for(int i = 0; i < chunkSize; i++) {
            for(int j = 0; j < chunkSize; j++) {
                Color c = pal[state[i][j]];
                Vector2 pos = getCell(origin, cellSize, (Vector2){i + 1, -j - 1});
                DrawRectangleV(pos, cell, c);
            }
        }
        drawGrid(origin, cellSize);

        prevMouse = mouse;
        frame = (frame + 1) % tick;
        EndDrawing();
    }

    CloseWindow();

  return 0;
}
