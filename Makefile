CC=gcc
CFLAGS=-Wall -Wextra -std=c99
CLIBS=-lraylib -lm

CFILES=sim.c game.c world.c
OBJ=sim.o game.o world.o

BIN=game

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BIN) $(OBJ)
