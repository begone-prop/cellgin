#include <stdio.h>
#include "hashmap.h"
#include "world.h"

static void freeChunk(chunk_t *);
static void printChunkList(chunk_t *);
static int resize(map_t *, float);

int updateChunk(map_t *hashmap, chunk_t *chunk, Vector2 cell, size_t chunkSize, int value, int which) {

    int updateNext = (which > 0);

    int *wstate =  updateNext ? chunk->nextState : chunk->state;
    size_t *alive = updateNext ? &chunk->newAlive : &chunk->alive;

    if(!chunk || !wstate) return 0;
    wstate[(int)cell.y + (chunkSize * (int)cell.x)] = value;


    if(value == 0) {
        int cond = (*alive == 0);
        *alive = !cond * (*alive - 1) + cond * 0;
    } else {
        if(*alive == 0) {
            Vector2 neighbours[8];
            getChunkNeighbours(neighbours, 8, chunk->index);
            for(short i = 0; i < 8; i++)
                insert(hashmap, neighbours[i], chunkSize);
        }

        chunk->timeToLive = TIME_TO_LIVE;
        *alive += *alive < (chunkSize * chunkSize);
    }

    return 1;
}

int getCellValue_(const chunk_t *chunk, Vector2 cell, size_t chunkSize) {
    if(!chunk || !chunk->state) return -1;
    return chunk->state[(int)cell.y + chunkSize * (int) cell.x];
}

int getCellValue(map_t hashmap, const chunk_t *chunk, Vector2 cell, size_t chunkSize) {
    if(!chunk || !chunk->state) return -1;
    int value = -1;
    Vector2 sign = VSIGN(chunk->index);
    Vector2 chunkIndex = chunk->index;

    int changed = 0;

    if(cell.x < 0 || cell.x >= chunkSize) {
        int cond = cell.x < 0;
        int idx = abs((int)cell.x) % chunkSize;
        cell.x = cond * (chunkSize - idx) + !cond * idx;
        chunkIndex.x += (cond * -1 + !cond * 1);
        chunkIndex.x += (chunkIndex.x == 0) * -sign.x;
        changed = 1;
    }

    if(cell.y < 0 || cell.y >= chunkSize) {
        int cond = cell.y < 0;
        int idx = abs((int)cell.y) % chunkSize;
        cell.y = cond * (chunkSize - idx) + !cond * idx;
        chunkIndex.y += (cond * -1 + !cond * 1);
        chunkIndex.y += (chunkIndex.y == 0) * -sign.y;
        changed = 1;
    }

    if(changed) {
        chunk_t *sel = find(hashmap, chunkIndex);
        if(sel) value = getCellValue_(sel, cell, chunkSize);
        else value = 0;
    } else {
        value = getCellValue_(chunk, cell, chunkSize);
    }

    return value;
}

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
    /*for(size_t j = 0; j < hashmap.size; j++) {*/
        /*if(hashmap.slots[j]) {*/
            /*fprintf(stderr, "[%02zu]:\t", j);*/
            /*printChunkList(hashmap.slots[j]);*/
        /*}*/
    /*}*/

    fprintf(stderr, "Size: %zu Items: %zu Load: %0.2f\n", hashmap.size,
            hashmap.taken,
            !hashmap.size ? 0 : (float) hashmap.taken / hashmap.size );
    fprintf(stderr, "####\n");
}

static void freeChunk(chunk_t *chunk) {
    free(chunk->state);
    free(chunk->nextState);
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
            hashmap->size > MIN_SIZE) {
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
                free(next->nextState);
                current->next = next->next;
                free(next);
                hashmap->taken--;
                break;
            }
        }
    }

    return 1;
}

chunk_t *insert(map_t *entry, Vector2 chunkidx, size_t chunkSize) {

    if((float) (entry->taken + 1) / entry->size >= GROW_THRESHOLD &&
            entry->size * 2 <= MAX_SIZE) {
        fprintf(stderr, "Growing table\n");
        resize(entry, 2);
    }

    size_t idx = map((int)chunkidx.x, (int)chunkidx.y) % entry->size;
    chunk_t *update = search(entry->slots[idx], chunkidx);

    if(update != NULL) return update;

    chunk_t *chunk = createChunk(chunkidx, chunkSize);
    if(!chunk) return NULL;

    chunk->next = entry->slots[idx];
    entry->slots[idx] = chunk;
    entry->taken++;
    return chunk;
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
    newChunk->newAlive = 0;
    newChunk->timeToLive = TIME_TO_LIVE;
    newChunk->state = calloc(chunkSize * chunkSize, sizeof(int));
    newChunk->nextState = calloc(chunkSize * chunkSize, sizeof(int));

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
