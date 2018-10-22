CC = gcc	
CFLAGS = -Wall -std=c99
LDFLAGS = -lSDL2main -lSDL2
OUT = menuTest

all: main.c
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f *.o $(OUT)