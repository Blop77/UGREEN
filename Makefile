CC      ?= cc
CFLAGS  ?= -O2 -Wall -Wextra -Wpedantic -std=c11
LDFLAGS ?=

SRC = src/main.c src/scan.c src/hash.c src/dupes.c src/ai.c
OBJ = $(SRC:src/%.c=build/%.o)
BIN = build/nasweep

.PHONY: all test clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

build/%.o: src/%.c src/*.h | build
	$(CC) $(CFLAGS) -c -o $@ $<

build:
	mkdir -p build

test: $(BIN)
	sh tests/run_tests.sh $(BIN)

clean:
	rm -rf build
