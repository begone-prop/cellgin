CC=gcc
CFLAGS=-Wall -Wextra -std=c99
CLIBS=-lraylib

CFILES=sim.c game.c
OBJ=sim.o game.o

BIN=game

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(BIN) $(OBJ)
