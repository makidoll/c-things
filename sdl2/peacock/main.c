#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_TITLE "Peacock"
#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 128
#define WINDOW_SIZE 4

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_Texture* texture[1];

	int state;
} Game;

void loadImage(Game* game, int id, char* file_path) {
	game->texture[id] = SDL_CreateTextureFromSurface(
		game->renderer, IMG_Load(file_path));
}

void init(Game* game) {
	if (SDL_Init(SDL_INIT_EVERYTHING)==0) {
		game->window = SDL_CreateWindow(
			WINDOW_TITLE,
			SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH*WINDOW_SIZE, WINDOW_HEIGHT*WINDOW_SIZE,
			SDL_WINDOW_SHOWN
		);

		game->renderer = SDL_CreateRenderer(
			game->window, -1, 
			SDL_RENDERER_ACCELERATED
		);

		// load images
		IMG_Init(IMG_INIT_PNG);
		loadImage(game, 0, "discord.png");

		game->state = 1;
	} else {
		game->state = -1;
	}
}

void drawImage(Game* game, int id, int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x; rect.y = y;
	rect.w = w; rect.h = h;

	SDL_RenderCopy(game->renderer, game->texture[id],
		NULL, NULL);
}

void draw(Game* game) {
	drawImage(game, 0, 0, 0, 64, 64);
	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	while (SDL_PollEvent(&game->event)) {
		switch(game->event.type) {
			case SDL_QUIT: game->state = 0; break;
		}
	}

	draw(game);
}

int main(int argc, char* argv[]){
	Game game;
	init(&game);

	while(game.state > 0) {
		update(&game);
	}

	SDL_DestroyWindow(game.window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}