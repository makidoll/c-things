CC = gcc
LINKS = -lGL -lSDL2main -lSDL2
OUT = shader-test

all: main.c
	$(CC) main.c -Wall $(LINKS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)