CC = gcc
OUT = snake

all: main.c
	$(CC) main.c -Wall -lncurses -o $(OUT)

run: all
	./$(OUT)

clean: 
	rm -f $(OUT)