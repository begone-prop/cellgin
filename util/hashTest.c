#include <stdio.h>
#include <stdint.h>
#include <math.h>

int map(int x, int y) {
    return (size_t)(((x | 0xAA) + ((y & 0x55) & 0x55)) | (((x | 0x55) + (y & 0xAA)) & 0xAA));
    /*return (int)((x | 0xAAAA) + (y & 0x5555) & 0x5555) | ((x | 0x5555) + (y & 0xAAAA) & 0xAAAA);*/
}

// method to seperate bits from a given integer 3 positions apart
uint64_t splitBy3(int a){
    uint64_t x = a & 0x1fffff; // we only look at the first 21 bits
    x = (x | x << 32) & 0x1f00000000ffff; // shift left 32 bits, OR with self, and 00011111000000000000000000000000000000001111111111111111
    x = (x | x << 16) & 0x1f0000ff0000ff; // shift left 32 bits, OR with self, and 00011111000000000000000011111111000000000000000011111111
    x = (x | x << 8) & 0x100f00f00f00f00f; // shift left 32 bits, OR with self, and 0001000000001111000000001111000000001111000000001111000000000000
    x = (x | x << 4) & 0x10c30c30c30c30c3; // shift left 32 bits, OR with self, and 0001000011000011000011000011000011000011000011000011000100000000
    x = (x | x << 2) & 0x1249249249249249;
    return x;
}

uint64_t map2(int x, int y) {
    uint64_t answer = 0;
    answer |= splitBy3(x) | splitBy3(y) << 1;
    return answer;
}


int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    size_t tableSize = BUFSIZ;
    int box = 10;

    size_t chunkCount = 0;
    for(int x = -box; x <= box; x++) {
        for(int y = -box; y <= box; y++) {
            if(x == 0 || y == 0) continue;
            /*int idx = map(x, y) % tableSize;*/
            uint64_t idx = map2(x, y) % tableSize;
            printf("(x: %i, y: %i): %lu\n", x, y, idx);
            chunkCount++;
        }
    }

    return 0;
}
