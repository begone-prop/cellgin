#ifndef SIM_C_
#define SIM_C_

#include <stddef.h>
#include "world.h"
#include "hashmap.h"

#define IS_BOUND(X, DX) ((DX) >= (X) || (DX) < 0)
#define IS_NEIGHBOUR(X, DX, DY) !((IS_BOUND((X), (DX))) || (IS_BOUND((X), (DY))))

size_t countNeighbours(map_t, const chunk_t *, size_t, Vector2);
void nextGeneration(map_t *, chunk_t *, size_t);
#endif
