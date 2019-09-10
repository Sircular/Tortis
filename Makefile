CC=clang
CFLAGS=-Weverything -D_DEFAULT_SOURCE
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

