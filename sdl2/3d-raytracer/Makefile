CC = gcc
OUT = 3d_raytracer

LINKS = -lm -lSDL2main -lSDL2
ifeq ($(OS),Windows_NT)
	LINKS = -lmingw32 -lSDL2main -lSDL2
endif

all: main.c
	$(CC) main.c $(LINKS) -Wall -o "$(OUT)"

run: all
	"./$(OUT)"