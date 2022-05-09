#ifndef SIM_C_
#define SIM_C_

#include <stddef.h>
#include "world.h"
#include "hashmap.h"

#define IS_BOUND(X, DX) ((DX) >= (X) || (DX) < 0)
#define IS_NEIGHBOUR(X, DX, DY) !((IS_BOUND((X), (DX))) || (IS_BOUND((X), (DY))))

void oneState(int *, size_t);
void zeroState(int *, size_t);
size_t countNeighbours(map_t, const chunk_t *, size_t, Vector2);
int nextState(size_t x, size_t y, int board[][y], int next[][y]);
void printBoard(size_t x, size_t y, int board[][y]);
void copyState(size_t x, size_t y, const int board[][y], int board2[][y]);
#endif
