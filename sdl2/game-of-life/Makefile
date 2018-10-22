CC = gcc	
CFLAGS = -Wall -std=c99
LDFLAGS = -lSDL2main -lSDL2
OUT = gol

all: main.c
	$(CC) main.c $(CFLAGS) $(LDFLAGS) -o $(OUT)

taz: taz.c
	$(CC) taz.c $(CFLAGS) $(LDFLAGS) -o $(OUT)
	
clean:
	rm -f *.o $(OUT)