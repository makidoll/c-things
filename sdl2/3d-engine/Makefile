CC = gcc
LDFLAGS = -lm -lSDL2main -lSDL2
ifeq ($(OS),Windows_NT)
	LDFLAGS = -lmingw32 -lm -lSDL2main -lSDL2
endif
CFLAGS = -Wall
OBJ = main.c
OUT = engine

all: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)