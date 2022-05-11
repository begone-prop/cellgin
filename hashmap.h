#ifndef HASHMAP_C_
#define HASHMAP_C_

#ifndef GROW_THRESHOLD
#define GROW_THRESHOLD 0.7
#endif

#ifndef SHRINK_THRESHOLD
#define SHRINK_THRESHOLD 0.25
#endif

#ifndef MIN_SIZE
#define MIN_SIZE 8
#endif

#ifndef MAX_SIZE
#define MAX_SIZE 8192
#endif

#include <stdint.h>
#include <stddef.h>
#include <raymath.h>
#include <stdlib.h>

typedef struct chunk_t {
    Vector2 index;
    int *state;
    int *nextState;
    size_t alive;
    size_t newAlive;
    struct chunk_t *next;
} chunk_t;

typedef struct map_t {
    chunk_t **slots;
    size_t size;
    size_t taken;
} map_t;

uint64_t map(int x, int y);
map_t createMap(size_t);
chunk_t *createChunk(Vector2, size_t);
int freeChunkList(chunk_t *);
chunk_t *search(chunk_t *head, Vector2 index);
chunk_t *insert(map_t *, Vector2, size_t);
int deleteMap(map_t *);
int updateChunk(map_t *, chunk_t *, Vector2, size_t, int);
int getCellValue_(const chunk_t *, Vector2, size_t);
int getCellValue(map_t , const chunk_t *, Vector2, size_t);
void printMap(map_t);
int drop(map_t *, Vector2);
chunk_t *find(map_t, Vector2);
#endif
