CC=clang
CFLAGS=-Weverything -Wno-padded
LIBS=-lncurses
BIN=tortis

SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(BIN)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN) $(OBJECTS)

