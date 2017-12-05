#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "vectors.h"

#ifdef _WIN32
	#include <windows.h> // Sleep
#else
	#include <unistd.h> // usleep
#endif

#define TITLE "Maki's Coralreef"
#define WIDTH 16
#define HEIGHT 16
#define SIZE 24

#define UPDATE 1000/60

#define ROOT_MAX 256

#define FOOD_MAX 16
#define FOOD_SPAWN 8
// Modulo with frame; set to 1 to prove FOOD_MAX works.
// I actually started smiling when I saw how little CPU
// and memory this program uses. AAH I </3 JS and <3 C.

typedef struct {
	int alive;
	Vec2 pos;
} Cell;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int state;
	int frame;

	Cell food[FOOD_MAX];
	Cell root[ROOT_MAX];
} Game;

void reset(Game* game) {
	for (int i=0; i<FOOD_MAX; i++) {
		game->food[i].alive = 0;
	}

	for (int i=0; i<ROOT_MAX; i++) {
		game->root[i].alive = 0;
	}

	game->root[0].alive = 1;
	game->root[0].pos = vec2(WIDTH/2, HEIGHT-1);
}

void initGame(Game* game) {

	SDL_Init(SDL_INIT_EVERYTHING);
	srand(time(NULL));

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

	reset(game);
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

	for (int i=0; i<ROOT_MAX; i++) {
		if (game->root[i].alive) {
			SDL_SetRenderDrawColor(game->renderer, 255,119,168,255);
			SDL_Rect rect;
			rect.x = game->root[i].pos.x*SIZE;
			rect.y = game->root[i].pos.y*SIZE;
			rect.w = rect.h = SIZE;
			SDL_RenderFillRect(game->renderer, &rect);
		} else {
			break;
		}
	}

	SDL_RenderPresent(game->renderer);
}

void spawnFood(Game* game) {
	for (int i=0; i<FOOD_MAX; i++) {
		if (game->food[i].alive == 0) {
			game->food[i].alive = 1;
			game->food[i].pos = vec2(rand()%WIDTH, 0);
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
		// CODE MEISTER Tazzo, the Yukie lover...
		//Vec2 f = game->food[i];
		// End my life...

		// Move food

		if (rand()%10 < 6) {
			game->food[i].pos.y++;
		}

		if (rand()%10 < 2) {
			if (rand()%10 < 5) {	
				game->food[i].pos.x++;
			} else {
				game->food[i].pos.x--;
			}
		}

		// Kill food (ahem pointer needed much pls)
		if (
			game->food[i].pos.x < 0 || game->food[i].pos.x >= WIDTH ||
			game->food[i].pos.y < 0 || game->food[i].pos.y >= HEIGHT
		) {
			game->food[i].alive = 0;
		}
	}

}

int countFood(Game* game) {
	int f = 0;
	for (int i=0; i<FOOD_MAX; i++) {
		f += game->food[i].alive;
	}
	return f;
}

int countRoot(Game* game) {
	int r = 0;
	for (int i=0; i<ROOT_MAX; i++) {
		r += game->root[i].alive;
	}
	return r;
}

void addRoot(Game* game, Vec2 v) {
	for (int i=0; i<ROOT_MAX; i++) {
		if (game->root[i].alive == 0) {
			game->root[i].alive = 1;
			game->root[i].pos = v;
			break;
		}
	}
}

void updateRoot(Game* game) {

	for (int r=0; r<ROOT_MAX; r++) {
		// If not alive
		if (game->root[r].alive == 1) {

			// Check if at top
			if (game->root[r].pos.y < 2) {
				reset(game);
				break;
			}

			// Check all foods for collision
			for (int f=0; f<FOOD_MAX; f++) {
				if (game->food[f].alive && (
					(game->food[f].pos.x == game->root[r].pos.x-1 &&
					game->food[f].pos.y == game->root[r].pos.y)
						||
					(game->food[f].pos.x == game->root[r].pos.x+1 &&
					game->food[f].pos.y == game->root[r].pos.y)
						||
					(game->food[f].pos.x == game->root[r].pos.x &&
					game->food[f].pos.y == game->root[r].pos.y-1)
				)) {
					game->food[f].alive = 0;
					addRoot(game, game->food[f].pos);
				}
			}
		} else {
			break;
		}
	}
}

void updateGame(Game* game) {

	// Food
	updateFood(game);
	if (game->frame % FOOD_SPAWN == 0) {
		spawnFood(game);
	}

	// Root
	updateRoot(game);

	// Drawing and debug
	drawGame(game);
	printf("\033[2J\n");
	printf("Frame: %d\n", game->frame); 
	printf("Foods: %d\n", countFood(game)); 
	printf("Roots: %d\n", countRoot(game)); 

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
		case SDLK_SPACE:
			reset(game);
			break;
	}

	// Update again!
	//char* title = game->frame+"0";
	//SDL_SetWindowTitle(game->window, title);
	
	++game->frame;
	#ifdef _WIN32
		Sleep(UPDATE);
	#else
		usleep(UPDATE*1000);
	#endif
	if (game->state > 0) updateGame(game);
}

int main(int argc, char* argv[]) {

	Game game;
	initGame(&game);

	updateGame(&game);

	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}