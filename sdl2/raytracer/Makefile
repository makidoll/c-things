CC = gcc
OUT = Raytracing

LINKS = -lm -lSDL2main -lSDL2
ifeq ($(OS),Windows_NT)
	LINKS += -lmingw32
endif

all: main.c
	$(CC) main.c -lm -lSDL2main -lSDL2 -Wall -o $(OUT)

run: all
	./$(OUT)