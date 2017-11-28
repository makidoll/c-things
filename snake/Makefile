CC = gcc
OBJS = main.c 
CFLAGS = -Wall -c
LDFLAGS = -lncurses
OUT = snake

all: main.o
	$(CC) main.o $(LDFLAGS) -o $(OUT)

main.o: main.c
	$(CC) $(CFLAGS) $(OBJS) -o main.o

clean: 
	rm -f *.o