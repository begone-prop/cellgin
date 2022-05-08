#ifndef SIM_C_
#define SIM_C_

#include <stddef.h>

#define IS_BOUND(X, DX) ((DX) >= (X) || (DX) < 0)
#define IS_NEIGHBOUR(X, Y, DX, DY) !((IS_BOUND((X), (DX))) || (IS_BOUND((Y), (DY))))

void zeroState(size_t x, size_t y, int board[][y]);
size_t countNeighbours(size_t x, size_t y, int board[][y], int cx, int cy);
int nextState(size_t x, size_t y, int board[][y], int next[][y]);
void printBoard(size_t x, size_t y, int board[][y]);
void copyState(size_t x, size_t y, const int board[][y], int board2[][y]);
#endif
