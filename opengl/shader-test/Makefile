CC = gcc
LINKS = -lGL -lSDL2main -lSDL2
FLAGS  = -Wall $(LINKS)
OUT = shader-test

all: main.c
	$(CC) main.c $(FLAGS) -o $(OUT)

run: all
	./$(OUT)