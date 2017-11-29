CC = gcc
CFLAGS = -Wall
LDFLAGS = -lSDL2main -lSDL2
FLAGS = $(CFLAGS) $(LDFLAGS)

OUT = coralreef

all: main.c vectors.o
	$(CC) main.c vectors.o $(FLAGS) -o $(OUT)

vectors.o: vectors.c vectors.h
	$(CC) -c vectors.c $(FLAGS)

# extra

run: all
	./$(OUT)

clean:
	rm -f *.o $(OUT)