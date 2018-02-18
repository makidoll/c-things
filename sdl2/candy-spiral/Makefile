CC = gcc
OUT = candy-spiral

all: main.c 
	$(CC) main.c -Wall -lSDL2main -lSDL2 -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)