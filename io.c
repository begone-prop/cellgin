#define _XOPEN_SOURCE 500

#include "./io.h"
#include <stdio.h>

static const uint8_t golFileSigniture[] = {0, 9, 22, 37, 0, 11, 4, 20};
static const size_t sigSize = sizeof(golFileSigniture) / sizeof(golFileSigniture[0]);
static const uint16_t chunkSize = 10;

/*
   File sig.
   chunk_size
   -- chunk start --
   index
   data
   -- chunk end --
*/

ssize_t writeGameState(const board_t *board, const char *fileName) {
    ssize_t bytesWritten = 0;
    mode_t perms = S_IRUSR | S_IWUSR | S_IROTH | S_IRGRP;
    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, perms);

    if(fd == -1) return -1;

    off_t offset = 0;
    offset += pwrite(fd, golFileSigniture, sigSize, offset);
    offset += pwrite(fd, &chunkSize, sizeof(chunkSize), offset);

    for(size_t idx = 0; idx < board->chunks.size; idx++) {
        if(!board->chunks.slots[idx]) continue;
        for(chunk_t *current = board->chunks.slots[idx]; current; current = current->next) {
            fprintf(stderr, "I: {x: %0.f, y: %0.f}\n", current->index.x, current->index.y);
            offset += pwrite(fd, &current->index, sizeof(current->index), offset);
            offset += pwrite(fd, current->state, (chunkSize * chunkSize * sizeof(current->state[0])), offset);
        }
    }

    close(fd);
    return bytesWritten;
}
