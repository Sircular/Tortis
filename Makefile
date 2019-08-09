CC=clang
CFLAGS=-Weverything -Wno-padded
LIBS=-lncurses
BIN=tortis

SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:.c=.o)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(BIN)

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN) $(OBJECTS)

