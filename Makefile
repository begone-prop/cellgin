CC=gcc
CFLAGS=-Wall -Wextra -std=c99
CLIBS=-lraylib -lm

OBJ=sim.o game.o world.o hashmap.o

BIN=game

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CLIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(BIN) $(OBJ)
