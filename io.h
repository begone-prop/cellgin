#ifndef GOL_IO_H_
#define GOL_IO_H_

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "world.h"

ssize_t writeGameState(const board_t *, const char *);
#endif
