#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define RENDER_SIZE 128
#define WINDOW_SIZE 512

#define PI 3.1415

#define TITLE "Maki learns Raytracing"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int screen[RENDER_SIZE][RENDER_SIZE];
	int state; // 0 is off, 1 is on
	int size;
} Game;

void init(Game* game) {
	SDL_Init(SDL_INIT_EVERYTHING);

	game->window = SDL_CreateWindow(
		TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_SIZE, WINDOW_SIZE, SDL_WINDOW_SHOWN
	);

	game->renderer = SDL_CreateRenderer(
		game->window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	for (int y=0; y<RENDER_SIZE; ++y) {
		for (int x=0; x<RENDER_SIZE; ++x) {
			game->screen[y][x] = 0;
		}
	}

	game->state = 1;
	game->size = WINDOW_SIZE/RENDER_SIZE;
}

void render(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderDrawColor(game->renderer, 255,255,255,255);
	for (int y=0; y<RENDER_SIZE; ++y) {
		for (int x=0; x<RENDER_SIZE; ++x) {

			float uv[2] = {
				x/(RENDER_SIZE/2)-1,
				y/(RENDER_SIZE/2)-1
			};

			float d = 128;

			SDL_SetRenderDrawColor(game->renderer, d,d,d,255);
			//if (game->screen[x][y] >= 1) {
				SDL_Rect rect;
				rect.x = x*game->size; rect.y = y*game->size;
				rect.w = rect.h = game->size;
				SDL_RenderFillRect(game->renderer, &rect);
			//}
		}
	}

	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	render(game);
	SDL_PollEvent(&game->event);
	switch(game->event.type) {
		case SDL_QUIT:
			game->state = 0;
			break;
	}
}

int main(int argc, char* argv []) {
	
	Game game;
	init(&game);

	while(game.state > 0) {
		update(&game);	
	}	

	SDL_DestroyWindow(game.window);
    SDL_Quit();
	return 0;
}