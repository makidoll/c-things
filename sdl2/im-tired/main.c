#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "I'm Tired"
#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768

typedef struct {
	SDL_Surface* surface;
	int w; int h;
} Image;

typedef struct {
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Event event;

	SDL_Surface* image[1];

	clock_t start_time; 
	int state;
} Game;

void init(Game* game) {
	if (SDL_Init(SDL_INIT_EVERYTHING)==0) {
		game->window = SDL_CreateWindow(
			WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
		);

		game->surface = SDL_GetWindowSurface(game->window);

		game->image[0] = SDL_LoadBMP("brian.bmp");

		game->start_time = clock();
		game->state = 1;
	} else {
		game->state = -1;
		printf("Couldn't load SDL2\n");
	}
}

void drawImage(SDL_Surface* image, SDL_Surface* surface, int x, int y, int w, int h) {
	SDL_Rect dest;
	dest.x = x; dest.y = y; 
	dest.w = w; dest.h = h;
	SDL_BlitScaled(image, NULL, surface, &dest);
}

void drawBg(SDL_Surface* image, SDL_Surface* surface, float time) {
	int w = WINDOW_WIDTH/8;
	int h = WINDOW_WIDTH/8;
	float wt = fmod(time, w);
	float ht = fmod(time, h);

	for (int y=0; y<9; ++y) {
		for (int x=0; x<9; ++x) {
			drawImage(image, surface, (wt+x*w)-w, (ht+y*h)-h, w, h); 
		}
	}
}

void update(Game* game) {
	// time
	float time = (float)(clock()-game->start_time)/100000.0F;

	// events
	while (SDL_PollEvent(&game->event)) {
		switch(game->event.type) {
			case SDL_QUIT: game->state = 0; break;
		}
	}

	// draw

	// bg
	//drawBg(game->image[0], game->surface, time);

	// cat
	drawImage(game->image[0], game->surface, 
		WINDOW_WIDTH/2-64 + sin(time*0.04)*WINDOW_WIDTH/3, 
		WINDOW_HEIGHT/2-64 + cos(time*0.1)*WINDOW_HEIGHT/6, 
		256, 192+sin(time*0.2)*96);
	SDL_UpdateWindowSurface(game->window);

}

int main(int argc, char const *argv[]) {
	Game game;
	init(&game);
	while (game.state) update(&game);
	SDL_DestroyWindow(game.window);
	SDL_FreeSurface(game.surface);
	SDL_Quit();
	return 0;
}