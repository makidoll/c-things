CC = gcc
FLAGS = -Wall -lSDL2main -lSDL2
OUT = snake

all: main.c game.o
	$(CC) main.c game.o $(FLAGS) -o $(OUT)

game.o: game.c game.h
	$(CC) -c game.c $(FLAGS) 

# Extra

clean:
	rm -f *.o

run: all
	./$(OUT)