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
static const size_t rectSize = 20;

int main(int argc, char **argv) {
    SetTargetFPS(10);
    InitWindow(width, height, "Game of Life");
    bool animate = false;

    int fd = open("/dev/null", O_WRONLY);

    const size_t rectSize = 20;
    size_t sizeX = rectSize;
    size_t sizeY = rectSize;

    double dx = (double) width / sizeX;
    double dy = (double) height / sizeY;

    size_t cellsSize = sizeX * sizeY;
    int cells[sizeY][sizeX];
    int nextCells[sizeY][sizeX];

    zeroState(sizeX, sizeY, cells);
    zeroState(sizeX, sizeY, nextCells);

    Vector2 prev = {0, 0};

    while(!WindowShouldClose()) {

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            animate = false;
            SetTargetFPS(60);
            Vector2 mouse = GetMousePosition();

            if(mouse.x < 0 || mouse.x > width) goto draw;
            if(mouse.y < 0 || mouse.y > height) goto draw;

            int x = mouse.x / dx;
            int y = mouse.y / dy;

            if(x > sizeX || x < 0) continue;
            if(y > sizeY || y < 0) continue;

            if(!(x == prev.x && y == prev.y)) {
                cells[x][y] = 1; //cells[x][y] ? 0 : 1;
            }

            prev.x = x;
            prev.y = y;
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            zeroState(sizeX, sizeY, cells);
        }

        if(IsKeyPressed(KEY_SPACE)) {
            animate = !animate;
        }

        draw:
        BeginDrawing();
        ClearBackground(RAYWHITE);

        size_t idx = 0;
        for(int x = 0; x < sizeX; x++) {
            for(int y = 0; y < sizeY; y++) {
                DrawRectangle(dx * x, dy * y, dx, dy, cells[x][y] ? WHITE : BLACK);
                /*DrawRectangleLines(dx * x, dy * y, dx, dy, WHITE);*/
            }
        }

        if(animate) {
            SetTargetFPS(10);
            nextState(sizeX, sizeY, cells, nextCells);
            copyState(sizeX, sizeY, nextCells, cells);
        }

        EndDrawing();
    }

    CloseWindow();
    /*close(fd);*/
    return 0;
}
