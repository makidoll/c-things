CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm -lGL -lSDL2main -lSDL2
OUT = obj-test

OBJ = \
	main.o\
	obj.o

all: $(OBJ) 
	$(CC) $(OBJ) $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -rf $(OBJ) $(OUT) 

fresh: clean run 