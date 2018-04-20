CC = gcc
OUT = pea

LINKS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

all: main.c 
	$(CC) main.c -Wall $(LINKS) -o "$(OUT)"

run: all
	"./$(OUT)"