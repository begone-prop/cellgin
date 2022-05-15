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
                int value = (alive == 2 || alive == 3);
                updateChunk(hashmap, chunk, index, chunkSize, value, 1);
            } else {
                int value = (alive == 3);
                updateChunk(hashmap, chunk, index, chunkSize, value, 1);
            }
        }
    }
}
