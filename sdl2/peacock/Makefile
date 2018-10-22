CC = gcc
CFLAGS = -Wall
LDFLAGS = -lSDL2main -lSDL2 -lSDL2_image
OUT = pea

all: main.c 
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	$(OUT)

.PHONY: clean
clean:
	rm -f $(OUT)