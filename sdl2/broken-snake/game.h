#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "settings.h"

typedef struct {

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int x; int y;
	int dir; // up, down, left, right

	int state;
	int score;

	int screen[WIDTH][HEIGHT];

} Game;

void init(Game* game);
void update(Game* game);
void deinit(Game* game);

#endif 