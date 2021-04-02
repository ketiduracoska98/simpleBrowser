CC=gcc
CFLAGS=-std=c99 -Wall -Wextra
EXE=tema2

all: build

build: *.c
	$(CC) $^ $(CFLAGS) -o $(EXE)

run: $(EXE)
	./$(EXE) input.in output.out

clean:
	rm -f $(EXE)

.PHONY: clean run

