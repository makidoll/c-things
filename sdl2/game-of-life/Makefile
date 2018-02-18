CC = gcc	
CFLAGS = -Wall -std=c99
LDFLAGS = -lSDL2main -lSDL2
FLAGS = $(CFLAGS) $(LDFLAGS)

ifeq ($(OS),Windows_NT)
	WINCLUDE = -IC:\MinGW\include
	WLIBRARY = -LC:\MinGW\lib
	WFLAGS = -lmingw32 -Wl,-subsystem,windows -lSDL2main -lSDL2
	FLAGS = $(WINCLUDE) $(WLIBRARY) $(CFLAGS) $(WFLAGS)
endif

OUT = game_of_life

all: main.c
	$(CC) main.c $(FLAGS) -o $(OUT)

taz: taz.c
	$(CC) taz.c $(FLAGS) -o $(OUT)
	
clean:
ifeq ($(OS),Windows_NT)
	del /s *.o $(OUT)
else
	rm -f *.o $(OUT)
endif