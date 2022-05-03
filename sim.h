#ifndef SIM_C
#define SIM_C

#include <stddef.h>

#define WORLD_X 1000
#define WORLD_Y 1000

#define IS_BOUND(X, DX) ((DX) >= (X) || (DX) < 0)
#define IS_NEIGHBOUR(X, Y, DX, DY) !((IS_BOUND((X), (DX))) || (IS_BOUND((Y), (DY))))

void zeroState(size_t x, size_t y, int board[][y]);
size_t countNeighbours(size_t x, size_t y, int board[][y], int cx, int cy);
int nextState(size_t x, size_t y, int board[][y], int next[][y]);
void printBoard(size_t x, size_t y, int board[][y]);
void copyState(size_t x, size_t y, const int board[][y], int board2[][y]);
#endif
