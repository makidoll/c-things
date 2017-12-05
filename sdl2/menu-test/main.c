#include <stdio.h>
#include <SDL2/SDL.h>

#define TITLE "Maki's Menu"
#define WIDTH 48
#define HEIGHT 48
#define SIZE 8

typedef struct {
	
	int i;

} MainMenu;

typedef struct {
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	int screen[WIDTH][HEIGHT];

	int frame;
	int state;

	MainMenu menu;

} Game;

void addSprite(Game* game, char* name, int x, int y) {
	
	int c; int sx = x;
	FILE* file;
	file = fopen(name, "r");

	if (file) {
		while ((c = getc(file)) != EOF) {
			if (c == 10) { ++y; x=sx; } // new line
			if (c == 46) { ++x; } // "." transparent
			if (c >= 48 && c <= 55) { game->screen[x][y] = c-48; ++x; }
		} fclose(file);
	}

}

void initGame(Game* game) {

	SDL_Init(SDL_INIT_EVERYTHING);

	game->window = SDL_CreateWindow(
		TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH*SIZE, HEIGHT*SIZE, SDL_WINDOW_SHOWN
	);

	game->renderer = SDL_CreateRenderer(
		game->window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	game->frame = 0;
	game->state = 1;

	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			game->screen[x][y] = 0;
		}
	}
}

void clearScreen(Game* game, int i) {
	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			game->screen[x][y] = i;
		}
	}
}

void setColor(Game* game, int i) {
	int r, g, b;
	switch (i) { // matriax8c
		case 0: r=240; g=240; b=220; break;
		case 1: r=250; g=200; b=0  ; break;
		case 2: r=16 ; g=200; b=64 ; break;
		case 3: r=0  ; g=160; b=200; break;
		case 4: r=210; g=64 ; b=64 ; break;
		case 5: r=160; g=105; b=75 ; break;
		case 6: r=115; g=100; b=100; break;
		case 7: r=16 ; g=24 ; b=32 ; break;
	}
	SDL_SetRenderDrawColor(game->renderer, r,g,b,255);
}

void drawGame(Game* game) {

	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {

			setColor(game, game->screen[x][y]);

			SDL_Rect rect;
			rect.x = x*SIZE;
			rect.y = y*SIZE;
			rect.w = rect.h = SIZE;
			SDL_RenderFillRect(game->renderer, &rect);
		}
	}

	SDL_RenderPresent(game->renderer);

}

void gameLoop(Game* game) {

	clearScreen(game, 7);

	if (game->state == 1) { // menu
		addSprite(game, "spr/logo", 0, 0);
	}

	drawGame(game);

	while (SDL_PollEvent(&game->event)) {
		switch(game->event.type) {
			case (SDL_QUIT):
				game->state = 0;
				break;
		}

		switch(game->event.key.keysym.sym) {
			case (SDLK_ESCAPE):
				game->state = 0;
				break;
		}
	}

	//printf("\033[2J\n");
	//printf("Frame: %d\n", game->frame); 
	game->frame++;
}

int main(int argc, char* argv[]) {
	
	Game game;
	initGame(&game);
	while (game.state > 0) {
		gameLoop(&game);
	}
	SDL_DestroyWindow(game.window);
    SDL_Quit();
	return 0;
}