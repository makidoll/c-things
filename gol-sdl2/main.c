#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define WIDTH 128
#define HEIGHT 128
#define SIZE 4
#define UPDATE 1000/60
#define TITLE "Maki learns SDL2 and does the Game of Life"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int cells[HEIGHT][WIDTH];
	int pre_cells[HEIGHT][WIDTH];
	int frame;

	int state;
} Game;

void drawCells(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderDrawColor(game->renderer, 255,255,255,255);

	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			if (game->cells[y][x] > 0) {
				//SDL_RenderDrawPoint(game->renderer, x, y);
				SDL_Rect rect;
				rect.x = x*SIZE; rect.y = y*SIZE;
				rect.w = rect.h = SIZE;
				SDL_RenderFillRect(game->renderer, &rect);
			}
		}
	}
	
	SDL_RenderPresent(game->renderer);
}

void addRpentomino(Game* game, int y, int x) {
	game->cells[y][x+1] = 1;
	game->cells[y][x+2] = 1;
	game->cells[y+1][x] = 1;
	game->cells[y+1][x+1] = 1;
	game->cells[y+2][x+1] = 1;
}

void addGlider(Game* game, int y, int x) {
	game->cells[y][x+1] = 1;
	game->cells[y+1][x+2] = 1;
	game->cells[y+2][x] = 1;
	game->cells[y+2][x+1] = 1;
	game->cells[y+2][x+2] = 1;
}

int getPreNeighbours(Game* game, int yy, int xx) {
	int neighbours = 0;

	for (int y=-1; y<2; y++) {
		for (int x=-1; x<2; x++) {
			if (
				(y != 0 || x != 0) &&
				(xx+x > -1 && xx+x < WIDTH) &&
				(yy+y > -1 && yy+y < HEIGHT)
			) {
				if (game->pre_cells[yy+y][xx+x]) ++neighbours;
			}
		}
	}

	return neighbours;
}

void update(Game* game) {

	// Draw frame
	drawCells(game);

	// Update pre_cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			game->pre_cells[y][x] = game->cells[y][x];
		}
	}

	// Evaluate pre_cells into cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			int n = getPreNeighbours(game, y, x);
			if (game->pre_cells[y][x]) {
				// Alive
				if (n < 2 || n > 3) game->cells[y][x] = 0;
			} else {
				// Dead
				if (n == 3) game->cells[y][x] = 1;
			}
		}
	}

	// SDL Event
	SDL_PollEvent(&game->event);
	switch(game->event.type) {
		case SDL_QUIT:
			game->state = 0;
			break;
	}

	// Update again!
	++game->frame;
	//SDL_SetWindowTitle(game->window, TITLE);
	usleep(UPDATE*1000);
	if (game->state > 0) update(game);
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// Init game
	Game game;

	game.window = SDL_CreateWindow(
		TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH*SIZE, HEIGHT*SIZE, SDL_WINDOW_SHOWN
	);

	game.renderer = SDL_CreateRenderer(
		game.window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	game.frame = 0;
	game.state = 1;

	// Cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			game.cells[y][x] = 0;
			game.pre_cells[y][x] = 0;
		}
	}

	// Start it
	addRpentomino(&game, HEIGHT/2-2, WIDTH/2-2);
	//addGlider(&game, 0, 0);

	update(&game);

	SDL_DestroyWindow(game.window);
    SDL_Quit();
	return 0;
}
