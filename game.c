#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <raylib.h>
#include <fcntl.h>
#include "sim.h"

static const int width = 1000;
static const int height = 1000;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    SetTargetFPS(10);
    InitWindow(width, height, "Game of Life");
    bool animate = false;

    Color pal[] = {BLACK, WHITE};
    bool black = true;

    const size_t rectSize = 25;
    size_t sizeX = rectSize;
    size_t sizeY = rectSize;

    double dx = (double) width / sizeX;
    double dy = (double) height / sizeY;

    /*size_t cellsSize = sizeX * sizeY * 16;*/
    int cells[sizeY][sizeX];
    int nextCells[sizeY][sizeX];

    zeroState(sizeX, sizeY, cells);
    zeroState(sizeX, sizeY, nextCells);

    Vector2 prevCell = {0, 0};

    bool hold = false;
    while(!WindowShouldClose()) {

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            animate = false;
            SetTargetFPS(60);
            Vector2 mouse = GetMousePosition();

            int x = mouse.x / dx;
            int y = mouse.y / dy;

            if(IS_BOUND(width, mouse.x)) goto draw;
            if(IS_BOUND(height, mouse.y)) goto draw;

            if(IS_BOUND(sizeX, (size_t) x)) goto draw;
            if(IS_BOUND(sizeY, (size_t) y)) goto draw;

            if(hold && !(x == prevCell.x && y == prevCell.y)) {
                cells[x][y] = 1;
            }

            if(!hold) cells[x][y] = !cells[x][y];

            prevCell.x = x;
            prevCell.y = y;
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            zeroState(sizeX, sizeY, cells);
        }

        if(IsKeyPressed(KEY_SPACE)) {
            animate = !animate;
        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        for(size_t x = 0; x < sizeX; x++) {
            for(size_t y = 0; y < sizeY; y++) {
                DrawRectangle(dx * x, dy * y, dx, dy, pal[black ^ cells[x][y]]);
                DrawRectangleLines(dx * x, dy * y, dx, dy, pal[!black ^ cells[x][y]]);
            }
        }

        if(animate) {
            SetTargetFPS(10);
            nextState(sizeX, sizeY, cells, nextCells);
            copyState(sizeX, sizeY, nextCells, cells);
        }

        hold = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
