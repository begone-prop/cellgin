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
#include <stdint.h>
#include "sim.h"
#include "world.h"
#include "hashmap.h"

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    InitWindow(width, height, "Game of Life");
    bool animate = false;

    size_t chunkSize = 10;
    Color pal[] = {BLACK, WHITE};
    static const size_t tick = 75;

    const size_t cellDens = 15;
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

    board_t board = {
        .origin = origin,
        .cellSize = cellSize,
        .chunkSize = chunkSize,
        .chunks = createMap(10)
    };

    Vector2 mouse, prevMouse;

    size_t frame = 0;
    Vector2 idx;
    idx.x = 0;
    idx.y = 0;

    while(!WindowShouldClose()) {
        mouse = GetMousePosition();

        if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            Vector2 delta = Vector2Subtract(mouse, prevMouse);
            board.origin = Vector2Add(board.origin, delta);
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            board.origin = screenCenter;
        }

        if(IsKeyPressed(KEY_SPACE)) {
            animate ^= 1;
        }

        /*if(IsKeyPressed(KEY_Z)) {*/
            /*zeroState(10, 10, state);*/
        /*}*/

        if(IsKeyPressed(KEY_P)) {
            printMap(board.chunks);
        }

        if(IsKeyPressed(KEY_D)) {
            for(size_t idx = 0; idx < board.chunks.size; idx++) {
                freeChunkList(board.chunks.slots[idx]);
            }

            board.chunks.size = 0;
            board.chunks.taken  = 0;
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(board, idx);
            /*animate = false;*/

            /*fprintf(stderr, "cell: (x: %i, y: %i) chunk: (x: %i, y: %i)\n",*/
                    /*(int)idx.x, (int)idx.y, (int)chunkidx.x, (int)chunkidx.y);*/

            insert(&board.chunks, chunkidx, idx);
            /*if(idx.x > 0 && idx.y > 0 && idx.x <= 10 && idx.y <= 10)*/
                /*state[(int)idx.x - 1][(int)idx.y - 1] ^= 1;*/

        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        /*if(animate && !(frame % tick)) {*/
            /*nextState(10, 10, state, newState);*/
            /*copyState(10, 10, newState, state);*/
        /*}*/

        /*for(int i = 0; (size_t)i < chunkSize; i++) {*/
            /*for(int j = 0; (size_t)j < chunkSize; j++) {*/
                /*Color c = pal[state[i][j]];*/
                /*Vector2 pos = getCellPosition(board, (Vector2){i + 1, -j - 1});*/
                /*DrawRectangleV(pos, (Vector2){cellSize, cellSize}, c);*/
            /*}*/
        /*}*/

        drawGrid(board);

        prevMouse = mouse;
        frame = (frame + 1) % tick;
        if(!(frame % tick)) fprintf(stderr, "dt: %f\r", GetFrameTime());
        EndDrawing();
    }


    deleteMap(&board.chunks);
    CloseWindow();

  return 0;
}
