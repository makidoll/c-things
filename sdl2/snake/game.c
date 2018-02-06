#include <stdio.h>
#include <SDL2/SDL.h>

#include "settings.h"
#include "game.h"
#include "text.h"

void init(Game* game) {

	SDL_Init(SDL_INIT_EVERYTHING);

	game->window = SDL_CreateWindow(
		TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH*SIZE, HEIGHT*SIZE, SDL_WINDOW_SHOWN
	);

	game->renderer = SDL_CreateRenderer(
		game->window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	game->x = WIDTH/2;
	game->y = HEIGHT/2;
	game->dir = 0; // up
	game->state = 1; // on

	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			game->screen[x][y] = 0;
		}
	}
} 

void clearScreen(Game* game) {
	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			game->screen[x][y] = 0;
		}
	}
}

void draw(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);

	SDL_SetRenderDrawColor(game->renderer, 255,255,255,255);
	SDL_Rect rect;
	for (int x=0; x<WIDTH; ++x) {
		for (int y=0; y<HEIGHT; ++y) {
			if (game->screen[x][y]>0) {
				rect.x = x*SIZE;
				rect.y = y*SIZE;
				rect.w = rect.h = SIZE;
				SDL_RenderFillRect(game->renderer, &rect);				
			}
		}
	}

	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	
	// Events
	SDL_PollEvent(&game->event);
	switch (game->event.type) {
		case SDL_QUIT: game->state = 0; break;
	}

	switch (game->event.key.keysym.sym) {
		case SDLK_ESCAPE: game->state = 0; break;

		case SDLK_UP: game->dir = 0; break;
		case SDLK_DOWN: game->dir = 1; break;
		case SDLK_LEFT: game->dir = 2; break;
		case SDLK_RIGHT: game->dir = 3; break;
	}

	// Game logic
	switch (game->dir) {
		case 0: --game->y; break;
		case 1: ++game->y; break;
		case 2: --game->x; break;
		case 3: ++game->x; break;
	}

	// Drawing!
	clearScreen(game);
	game->screen[game->x][game->y] = 1;
	printNum(game, 0, 0, 1);

	draw(game);
}

void deinit(Game* game) {
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}