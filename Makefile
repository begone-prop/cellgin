CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Wpedantic -ggdb
CLIBS=-lraylib -lm

OBJ=sim.o game.o world.o hashmap.o io.o

BIN=game

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(CLIBS)

clean:
	rm -f $(BIN) $(OBJ)
