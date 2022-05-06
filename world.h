#ifndef WORLD_H_
#define WORLD_H_

#include <raymath.h>
#include <stddef.h>

#define SIGN(A) (((A) > 0) * 1 + !((A) > 0) * -1)

static const int width = 1000;
static const int height = 1000;

void drawGrid(Vector2, size_t);
Vector2 getCellIndex(Vector2, size_t, Vector2);
Vector2 getCell(Vector2, size_t, Vector2);
#endif
