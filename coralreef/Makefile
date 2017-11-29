CC = gcc
FLAGS = -Wall -lSDL2main -lSDL2

OUT = coralreef

all: main.c vectors.o
	$(CC) main.c vectors.o $(FLAGS) -o $(OUT)

vectors.o: vectors.c vectors.h
	$(CC) -c vectors.c $(FLAGS)

build: clean all
	./$(OUT)

clean:
	rm -f *.o $(OUT)