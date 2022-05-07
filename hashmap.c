#include <stdio.h>
#include "hashmap.h"
#include "world.h"

static void freeChunk(chunk_t *);
void printChunkList(chunk_t *);
static int resize(map_t *, float);

int resize(map_t *hashmap, float ammount) {
    if(ammount < 0) return 0;
    size_t newSize = hashmap->size * ammount;
    map_t newMap = createMap(newSize);

    for(size_t i = 0; i < hashmap->size; i++) {
        chunk_t *current = hashmap->slots[i];
        while(current) {
            size_t idx = map(current->index.x, current->index.y) % newSize;
            chunk_t *next = current->next;
            current->next = newMap.slots[idx];
            newMap.slots[idx] = current;
            current = next;
        }
    }

    free(hashmap->slots);
    hashmap->slots = newMap.slots;
    hashmap->size = newSize;
    return 1;
}

void printChunkList(chunk_t *head) {
    if(!head) return;
    fprintf(stderr, "{x: %i, y: %i} %s",
            (int)head->index.x, (int)head->index.y, head->next ? "-> " : "\n");
    printChunkList(head->next);
}

void printMap(map_t hashmap) {
    fprintf(stderr, "####\n");
    for(size_t j = 0; j < hashmap.size; j++) {
        if(hashmap.slots[j]) {
            fprintf(stderr, "[%02zu]:\t", j);
            printChunkList(hashmap.slots[j]);
        }
    }

    fprintf(stderr, "Size: %zu Items: %zu Load: %0.2f\n", hashmap.size,
            hashmap.taken,
            !hashmap.size ? 0 : (float) hashmap.taken / hashmap.size );
    fprintf(stderr, "####\n");
}

static void freeChunk(chunk_t *chunk) {
    free(chunk->state);
    free(chunk);
    chunk = NULL;
}

int freeChunkList(chunk_t *head) {
    if(!head) return 0;
    freeChunkList(head->next);
    freeChunk(head);
    return 1;
}

int deleteMap(map_t * hashmap) {
    if(!hashmap) return 0;

    for(size_t idx = 0; idx < hashmap->size; idx++) {
        freeChunkList(hashmap->slots[idx]);
    }

    hashmap->size = 0;
    hashmap->taken = 0;
    return 1;
}

int drop(map_t *hashmap, Vector2 chunkIdx) {

    if((float) hashmap->taken / hashmap->size < SHRINK_THRESHOLD &&
            hashmap->size >= MIN_SIZE) {
        fprintf(stderr, "Shrinking table\n");
        resize(hashmap, 0.5);
    }

    size_t idx = map(chunkIdx.x, chunkIdx.y) % hashmap->size;
    chunk_t *head = hashmap->slots[idx];

    if(head && (chunkIdx.x == head->index.x && chunkIdx.y == head->index.y)) {
        chunk_t *next = head->next;
        freeChunk(head);
        hashmap->slots[idx] = next;
        next = hashmap->slots[idx];
        hashmap->taken--;
    } else {
        for(chunk_t *current = head; current; current = current->next) {
            chunk_t *next = current->next;
            if(next && (chunkIdx.x == next->index.x && chunkIdx.y == next->index.y)) {
                free(next->state);
                current->next = next->next;
                free(next);
                hashmap->taken--;
                break;
            }
        }
    }

    return 1;
}

int insert(map_t *entry, Vector2 chunkidx, Vector2 cell) {

    if((float) (entry->taken + 1) / entry->size >= GROW_THRESHOLD &&
            entry->size * 2 <= MAX_SIZE) {
        fprintf(stderr, "Growing table\n");
        resize(entry, 2);
    }

    if(chunkidx.x == 0 || chunkidx.y == 0 || cell.x == 0 || cell.y == 0)
        return 0;

    size_t idx = map(chunkidx.x, chunkidx.y) % entry->size;
    chunk_t *update = search(entry->slots[idx], chunkidx);

    if(update) {
        return 1;
    }


    chunk_t *chunk = createChunk(chunkidx, 10);
    if(!chunk) return 0;

    chunk->next = entry->slots[idx];
    entry->slots[idx] = chunk;
    entry->taken++;
    return 1;
}

map_t createMap(size_t mapSize) {
    map_t newMap;
    chunk_t **heads = malloc(mapSize * sizeof(chunk_t *));
    newMap.slots = heads;

    for(size_t i = 0; i < mapSize; i++)
        newMap.slots[i] = NULL;

    newMap.taken = 0;
    newMap.size = mapSize;
    return newMap;
}

chunk_t *createChunk(Vector2 index, size_t chunkSize) {
    chunk_t *newChunk = malloc(sizeof(chunk_t));
    newChunk->index = index;
    newChunk->alive = 0;
    newChunk->state = calloc(chunkSize * chunkSize, sizeof(int));

    if(!newChunk->state) {
        newChunk->index.x = 0;
        newChunk->index.y = 0;
    }

    newChunk->next = NULL;
    return newChunk;
}

chunk_t *search(chunk_t *head, Vector2 index) {
    if(!head) return NULL;
    if(index.x == head->index.x && index.y == head->index.y) return head;
    return search(head->next, index);
}

chunk_t *find(map_t hashmap, Vector2 index) {
    size_t idx = map(index.x, index.y) % hashmap.size;
    return search(hashmap.slots[idx], index);
}

/*
    Hash function taken from:
    https://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/
*/

static uint64_t splitBy3(int a){
    uint64_t x = a & 0x1fffff;
    x = (x | x << 32) & 0x1f00000000ffff;
    x = (x | x << 16) & 0x1f0000ff0000ff;
    x = (x | x << 8) & 0x100f00f00f00f00f;
    x = (x | x << 4) & 0x10c30c30c30c30c3;
    x = (x | x << 2) & 0x1249249249249249;
    return x;
}

uint64_t map(int x, int y) {
    uint64_t answer = 0;
    answer |= splitBy3(x) | splitBy3(y) << 1;
    return answer;
}
