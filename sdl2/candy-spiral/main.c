#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>

#define TITLE "Maki's Candy Spiral"
#define WIDTH 64
#define HEIGHT 64
#define SIZE 8

typedef struct {
	int r; int g; int b;	
} Color;

typedef struct {
	float x; float y;	
} UV;

typedef struct  {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	Color screen[WIDTH][HEIGHT];
	int state;
} Game;

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

	Color c;
	c.r = c.g = c.b = 0;
	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			game->screen[x][y] = c;
		}
	}

	game->state = 1;
}

void drawGame(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);

	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			SDL_SetRenderDrawColor(game->renderer,
				game->screen[x][y].r,
				game->screen[x][y].g,
				game->screen[x][y].b, 255);
			SDL_Rect rect;
			rect.x = x*SIZE;
			rect.y = y*SIZE;
			rect.w = rect.h = SIZE;
			SDL_RenderFillRect(game->renderer, &rect);
		}
	}

	SDL_RenderPresent(game->renderer);
}

void calcFrame(Game* game) {
	//int t = time(NULL);
	UV uv;
	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			uv.x = x:float/WIDTH; // -1 to 1
			game->screen[x][y].r = uv.x*255; 
		}
	}
	//printf("%d\n", time);
}

void gameLoop(Game* game) {
	calcFrame(game);
	drawGame(game);

	while(SDL_PollEvent(&game->event)) {
		switch(game->event.type) {
			case SDL_QUIT:
				game->state = 0;
				break;
		}
		switch(game->event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->state = 0;
				break;
		}
	}
}

int main(int argc, char* argv[]) {
	
	Game game;
	initGame(&game);

	while (game.state > 0) {
		gameLoop(&game);
	}

	return 0;
}