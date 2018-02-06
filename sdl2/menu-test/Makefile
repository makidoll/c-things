CC = gcc	
FLAGS = -Wall -lSDL2main -lSDL2
OUT = menu-test

all: main.c
	$(CC) main.c $(FLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT) 
