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

    unsigned long long generation = 0;

    InitWindow(width, height, "Game of Life");
    bool animate = false;
    SetTargetFPS(120);

    size_t chunkSize = 10;
    Color pal[] = {BLACK, WHITE};
    size_t tick = 15;

    const size_t cellDens = 20;
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
        if(scale != 0.0f) {
            if(board.cellSize <= 1) {
                if(scale > 0) board.cellSize += scale;
                else board.cellSize = 1;
            } else board.cellSize += (int)scale;
        }

        if(IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
            Vector2 delta = Vector2Subtract(mouse, prevMouse);
            board.origin = Vector2Add(board.origin, delta);
        }

        if(IsKeyPressed(KEY_SPACE)) {
            animate ^= 1;
        }

        if(IsKeyPressed(KEY_UP)) {
            if(tick > 1) tick--;
        }

        if(IsKeyPressed(KEY_DOWN)) {
            tick++;
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            fprintf(stderr, "Event!\n");
            idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);

            chunk_t *chosen = find(board.chunks, chunkidx);


            if(chosen) {
                for(size_t x = 0; x < board.chunkSize; x++) {
                    for(size_t y = 0; y < board.chunkSize; y++) {
                        fprintf(stderr, "%i", getCellValue_(chosen, (Vector2){x, y}, board.chunkSize));
                    }
                    fprintf(stderr, "\n");
                }
                fprintf(stderr, "Alive: %zu\n", chosen->alive);
            }
        }

        if(IsKeyPressed(KEY_P)) {
            printMap(board.chunks);
        }

        if(IsKeyPressed(KEY_ZERO)) {
            board.origin = screenCenter;
            board.cellSize = defCellSize;
        }

        if(IsKeyDown(KEY_F)) {
            Vector2 idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            chunk_t *chunk = find(board.chunks, chunkidx);
            if(chunk) {
                for(size_t x = 0; x < board.chunkSize; x++) {
                    for(size_t y = 0; y < board.chunkSize; y++) {
                        updateChunk(&board.chunks, chunk, (Vector2){x, y}, board.chunkSize, 1, 0);
                    }
                }
            }
        }

        if(IsKeyDown(KEY_Z)) {
            Vector2 idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            chunk_t *chunk = find(board.chunks, chunkidx);
            if(chunk) {
                memset(chunk->state, 0, chunkSize * chunkSize * sizeof(int));
                chunk->alive = 0;
            }
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            idx = getCellIndex(board, mouse);
            Vector2 chunkidx = getChunkIndex(idx, board.chunkSize);
            Vector2 rel = getRelativeCellIndex(idx, board.chunkSize);
            Vector2 chunkPos = getChunkPosition(board, chunkidx);
            animate = false;

            fprintf(stderr, "x: %i, y: %i\n", (int)mouse.x, (int)mouse.y);
            fprintf(stderr,
                    "{x: %i, y: %i}: "
                    "(x: %i, y: %i)"
                    "[x: %i, y: %i]\n",
                    (int)chunkidx.x, (int)chunkidx.y,
                    (int)idx.x, (int)idx.y,
                    (int)rel.x, (int)rel.y
                    );

            chunk_t *found = insert(&board.chunks, chunkidx, board.chunkSize);
            if(found) {
                int val = getCellValue_(found, rel, board.chunkSize);
                updateChunk(&board.chunks, found, rel, board.chunkSize, (val ^= 1), 0);
            }
        }

        draw:
        BeginDrawing();
        ClearBackground(BLACK);

        drawCells(board);
        drawGrid(board);

        prevMouse = mouse;
        frame = (frame + 1) % tick;
        if(!(frame % tick)) fprintf(stderr, "dt: %f (%6i FPS) generation: %-llu\r",
                GetFrameTime(), GetFPS(), generation);

        if(animate && !(frame % tick)) {
            for(size_t idx = 0; idx < board.chunks.size; idx++) {
                if(!board.chunks.slots[idx]) continue;
                for(chunk_t *current = board.chunks.slots[idx]; current; current = current->next) {
                    nextGeneration(&board.chunks, current, board.chunkSize);
                }
            }

            for(size_t idx = 0; idx < board.chunks.size; idx++) {
                if(!board.chunks.slots[idx]) continue;
                for(chunk_t *current = board.chunks.slots[idx]; current; current = current->next) {
                    int *tmp = current->state;
                    current->state = current->nextState;
                    current->nextState = tmp;
                    current->alive = current->newAlive;

                    memset(current->nextState, 0, board.chunkSize * board.chunkSize);
                    current->newAlive = 0;

                    Vector2 neighbours[8];
                    size_t total = 0;
                    getChunkNeighbours(neighbours, 8, current->index);
                    for(short i = 0; i < 8; i++) {
                        chunk_t *neigh = find(board.chunks, neighbours[i]);
                        if(neigh) total += neigh->alive;
                    }

                    if(total == 0) {
                        if(current->timeToLive == 0)
                            drop(&board.chunks, current->index);

                        else current->timeToLive--;
                    }
                }
            }

            generation++;
        }

        EndDrawing();
    }


    deleteMap(&board.chunks);
    CloseWindow();

  return 0;
}
