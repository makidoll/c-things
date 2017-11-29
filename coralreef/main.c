#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "vectors.h"

#define TITLE "Maki's Coralreef"
#define WIDTH 32
#define HEIGHT 32
#define SIZE 12

#define UPDATE 1000/60

#define FOOD_MAX 16
#define FOOD_SPAWN 1
// Modulo with frame; set to 1 to prove FOOD_MAX works.
// I actually started smiling when I saw how little CPU
// and memory this program uses. AAH I </3 JS and <3 C.

typedef struct {
	int alive;
	Vec2 pos;
} Food;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int state;
	int frame;

	Food food[FOOD_MAX];
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

	game->state = 1;
	game->frame = 0;

	for (int i=0; i<FOOD_MAX; i++) {
		game->food[i].alive = 0;
	}
}

void drawGame(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 29,43,83,255);
	SDL_RenderClear(game->renderer);

	for (int i=0; i<FOOD_MAX; i++) {
		if (game->food[i].alive) {
			SDL_SetRenderDrawColor(game->renderer, 131,118,156,255);
			SDL_Rect rect;
			rect.x = game->food[i].pos.x*SIZE;
			rect.y = game->food[i].pos.y*SIZE;
			rect.w = rect.h = SIZE;
			SDL_RenderFillRect(game->renderer, &rect);
		}
	}

	SDL_RenderPresent(game->renderer);
}

void spawnFood(Game* game) {
	for (int i=0; i<FOOD_MAX; i++) {
		if (game->food[i].alive == 0) {
			game->food[i].alive = 1;
			game->food[i].pos = vec2(2, 2);
			break;
		}
	}
}

void updateFood(Game* game) {

	for (int i=0; i<FOOD_MAX; i++) {
		// Not sure how to define a pointer...
		// See, I'm programming without the Internet
		// and... OH WAIT!! I HAVE THE C PROGRAMMING
		// LANGUAGE in PDF but actually I'll just ask
		// CODE MEISTER Tazzo, my Yukie lover...
		//Vec2 f = game->food[i];
		// End me...

		// Move food
		game->food[i].pos.y++;

		// Kill food (ahem pointer pls)
		if (
			game->food[i].pos.x < 0 || game->food[i].pos.x >= WIDTH ||
			game->food[i].pos.y < 0 || game->food[i].pos.y >= HEIGHT
		) {
			game->food[i].alive = 0;
		}

		// 
	}

}

int countFood(Game* game) {
	int f = 0;
	for (int i=0; i<FOOD_MAX; i++) {
		f += game->food[i].alive;
	}
	return f;
}

void updateGame(Game* game) {

	// Updating food
	updateFood(game);

	// Spawn food
	if (game->frame % FOOD_SPAWN == 0) {
		spawnFood(game);
	}

	// Drawing and debug
	drawGame(game);
	printf("\033[2J\n");
	printf("Frame: %d\n", game->frame); 
	printf("Foods: %d\n", countFood(game)); 

	// SDL Event
	SDL_PollEvent(&game->event);
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

	// Update again!
	//char* title = game->frame+"0";
	//SDL_SetWindowTitle(game->window, title);
	
	++game->frame;
	usleep(UPDATE*1000);
	if (game->state > 0) updateGame(game);
}

int main() {

	Game game;
	initGame(&game);

	updateGame(&game);

	SDL_DestroyWindow(game.window);
    SDL_Quit();
    return 0;
}