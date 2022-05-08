#define _DEFAULT_SOURCE

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
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
    static const size_t tick = 50;

    const size_t cellDens = 15;
    size_t cellSize = width / cellDens;
    const size_t defCellSize = cellSize;

    Vector2 origin = screenCenter;

    /*Vector2 origin = {*/
        /*.x = cellSize,*/
        /*.y = width - cellSize,*/
    /*};*/

    board_t board = {
        .origin = origin,
        .cellSize = cellSize,
        .chunkSize = chunkSize,
        .chunks = createMap(1024)
    };

    Vector2 mouse, prevMouse;

    size_t frame = 0;
    Vector2 idx;
    idx.x = 0;
    idx.y = 0;

    while(!WindowShouldClose()) {
        mouse = GetMousePosition();

        float scale = GetMouseWheelMove();
        if(scale != 0.0F) {
            board.cellSize += (int)scale;
        }

        if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            Vector2 delta = Vector2Subtract(mouse, prevMouse);
            board.origin = Vector2Add(board.origin, delta);
        }

        if(IsKeyPressed(KEY_SPACE)) {
            animate ^= 1;
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            fprintf(stderr, "Event!\n");
            idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);

            chunk_t *chosen = find(board.chunks, chunkidx);
            if(chosen) {
                for(size_t i = 0; i < board.chunkSize; i++) {
                    for(size_t j = 0; j < board.chunkSize; j++) {
                        fprintf(stderr, "%i%s", chosen->state[i + board.chunkSize * j],
                                j == board.chunkSize -1 ? "\n" : "");
                    }
                }
            }
        }

        if(IsKeyPressed(KEY_P)) {
            printMap(board.chunks);
        }

        if(IsKeyPressed(KEY_ZERO)) {
            board.origin = screenCenter;
            board.cellSize = defCellSize;
        }

        if(IsKeyPressed(KEY_F)) {
            Vector2 idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            Vector2 rel = getRelativeCellIndex(idx, board.chunkSize);
            chunk_t *chunk = find(board.chunks, chunkidx);
            if(chunk) {
                for(size_t i = 0; i < board.chunkSize; i++) {
                    for(size_t j = 0; j < board.chunkSize; j++) {
                        chunk->state[i + board.chunkSize * j] = 1;
                    }
                }
            }
        }

        if(IsKeyPressed(KEY_Z)) {
            Vector2 idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            Vector2 rel = getRelativeCellIndex(idx, board.chunkSize);
            chunk_t *chunk = find(board.chunks, chunkidx);
            if(chunk) memset(chunk->state, 0, board.chunkSize * board.chunkSize * sizeof(int));
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            Vector2 rel = getRelativeCellIndex(idx, board.chunkSize);
            Vector2 abso = getAbsoluteCellIndex(chunkidx, rel, board.chunkSize);
            /*animate = false;*/

            fprintf(stderr,
                    "{x: %i, y: %i}: "
                    "(x: %i, y: %i)"
                    "[x: %i, y: %i]"
                    "[x: %i, y: %i]\n",
                    (int)chunkidx.x, (int)chunkidx.y,
                    (int)idx.x, (int)idx.y,
                    (int)rel.x, (int)rel.y,
                    (int)abso.x, -(int)abso.y
                    );

            chunk_t *found = insert(&board.chunks, chunkidx, board.chunkSize);
            if(found) {
                updateChunk(found, rel, board.chunkSize);
            }

            if(VEDGE(rel, chunkSize)) {
                fprintf(stderr, "Edge\n");
            }
        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        /*if(animate && !(frame % tick)) {*/
        /*}*/

        for(size_t idx = 0; idx < board.chunks.size; idx++) {
            if(!board.chunks.slots[idx]) continue;
            for(chunk_t *current = board.chunks.slots[idx]; current; current = current->next) {
                for(int i = 0; (size_t)i < board.chunkSize; i++) {
                    for(int j = 0; (size_t)j < board.chunkSize; j++) {
                        int live = current->state[i + board.chunkSize * j];
                        Color prim = pal[live];
                        Color sec = pal[!live];
                        int x = (i + board.chunkSize * j) / board.chunkSize;
                        int y = (i + board.chunkSize * j) % board.chunkSize;
                        Vector2 abso = getAbsoluteCellIndex(current->index, (Vector2){x, y}, board.chunkSize);
                        Vector2 pos = getCellPosition(board, abso);

                        Rectangle rec = {
                            .x = pos.x,
                            .y = pos.y,
                            .height = board.cellSize,
                            .width = board.cellSize,
                        };

                        if(live) {
                            DrawRectangleRec(rec, prim);
                            DrawRectangleLinesEx(rec, 2.5f, sec);
                        }
                    }
                }
            }
        }

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
