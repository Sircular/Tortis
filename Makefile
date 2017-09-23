CC=clang
CFLAGS=-Weverything
LIBS=-lncurses
BIN=tetris

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN) $(OBJECTS)

