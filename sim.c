#include "./sim.h"

size_t countNeighbours(map_t hashmap, const chunk_t *chunk, size_t chunkSize, Vector2 cell) {
    size_t alive = 0;

    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            Vector2 newidx = { cell.x + i, cell.y + j, };
            alive += getCellValue(hashmap, chunk, newidx, chunkSize);
        }
    }

    return alive;
}

void nextGeneration(map_t *hashmap, chunk_t *chunk, size_t chunkSize) {
    for(size_t i = 0; i < chunkSize; i++) {
        for(size_t j = 0; j < chunkSize; j++) {
            Vector2 index = {i, j};
            int cellState = getCellValue_(chunk, index, chunkSize);
            size_t alive = countNeighbours(*hashmap, chunk, chunkSize, index);
            if(cellState) {
                /*if(alive == 2 || alive == 3) chunk->nextState[(int)index.y + chunkSize * (int)index.x] = 1;*/
                /*else chunk->nextState[(int)index.y + chunkSize * (int)index.x] = 0;*/
                if(alive == 2 || alive == 3) updateChunk(hashmap, chunk, index, chunkSize, 1, 1);
                else updateChunk(hashmap, chunk, index, chunkSize, 0, 1);
            } else {
                /*if(alive == 3) chunk->nextState[(int)index.y + chunkSize * (int)index.x] = 1;*/
                /*else chunk->nextState[(int)index.y + chunkSize * (int)index.x] = 0;*/
                if(alive == 3) updateChunk(hashmap, chunk, index, chunkSize, 1, 1);
                else updateChunk(hashmap, chunk, index, chunkSize, 0, 1);
            }
        }
    }
}
