#include <time.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Maki's Snake"
#define WINDOW_WIDTH 32
#define WINDOW_HEIGHT 32
#define WINDOW_SIZE 16
#define GAME_SPEED 1000/15
#define SNAKE_EXTEND 2

typedef enum {false, true} bool; 

typedef struct {
	int x; int y;
} Vec2;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	int state;

	int length;
	int extend_left;
	unsigned char dir; // w,a,s,d 
	
	Vec2 food;
	Vec2 snake[WINDOW_WIDTH*WINDOW_HEIGHT];
} Game;

void addSnake(Game* game, int x, int y) {
	game->snake[game->length].x = x;
	game->snake[game->length].y = y;
	++game->length;
}

void setFood(Game* game, int x, int y) {
	game->food.x = x;
	game->food.y = y;
}

void randomizeFood(Game* game) {
	setFood(game, rand()%WINDOW_WIDTH, rand()%WINDOW_HEIGHT);
}

void genSnake(Game* game, int x, int y, int size) {
	for (int i=0; i<size; ++i) {
		addSnake(game, x, y+i);
	}
}

void init(Game* game) {
	if (SDL_Init(SDL_INIT_EVERYTHING)==0) {
		game->window = SDL_CreateWindow(
			WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH*WINDOW_SIZE, WINDOW_HEIGHT*WINDOW_SIZE,
			SDL_WINDOW_SHOWN
		);

		game->renderer = SDL_CreateRenderer(
			game->window, -1, 
			SDL_RENDERER_ACCELERATED
		);

		game->state = 1;

		game->dir = 0;
		game->length = 0;
		game->extend_left = 0;
		memset(game->snake, 0, WINDOW_WIDTH*WINDOW_HEIGHT*sizeof(Vec2));
		
		genSnake(game, WINDOW_WIDTH/2, WINDOW_HEIGHT-7, 6);
		randomizeFood(game);

		srand(time(NULL));
	} else {
		game->state = -1;
	}
}

void drawRect(SDL_Renderer* renderer, int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x; rect.y = y;
	rect.w = w; rect.h = h;
	SDL_RenderFillRect(renderer, &rect);
}

void draw(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);

	// draw snake
	for (int i=0; i<game->length; i++) {
		SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
		drawRect(game->renderer,
			game->snake[i].x*WINDOW_SIZE,
			game->snake[i].y*WINDOW_SIZE,
			WINDOW_SIZE, WINDOW_SIZE);
	}

	// draw food
	
	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
	drawRect(game->renderer,
		game->food.x*WINDOW_SIZE,
		game->food.y*WINDOW_SIZE,
		WINDOW_SIZE, WINDOW_SIZE);

	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	// events
	while (SDL_PollEvent(&game->event)) {
		switch (game->event.type) {
			case SDL_QUIT: game->state = 0; break;
			case SDL_KEYDOWN: 
				switch (game->event.key.keysym.sym) {
					case SDLK_UP: 
						if (game->dir==1||game->dir==3) 
							game->dir = 0;
						break;
					case SDLK_LEFT: 
						if (game->dir==0||game->dir==2) 
							game->dir = 1;
						break;
					case SDLK_DOWN: 
						if (game->dir==1||game->dir==3) 
							game->dir = 2;
						break;
					case SDLK_RIGHT: 
						if (game->dir==0||game->dir==2) 
							game->dir = 3;
						break;
				}
			break;
		}
	}

	// snake
	Vec2 new_snake; // new snake
	Vec2 old_snake; // old snake
	for (int i=0; i<game->length; i++) {
		// moving snake to new loc
		if (i==0) {
			if (game->snake[i].x==game->food.x &&
				game->snake[i].y==game->food.y) {
				game->extend_left += SNAKE_EXTEND;
				randomizeFood(game);
			}
			
			new_snake = game->snake[i];
			switch (game->dir) {
				case 0: --game->snake[i].y; break;
				case 1: --game->snake[i].x; break;
				case 2: ++game->snake[i].y; break;
				case 3: ++game->snake[i].x; break;
			}
			if (game->snake[i].x<0) game->snake[i].x=WINDOW_WIDTH-1;
			if (game->snake[i].x>WINDOW_WIDTH-1) game->snake[i].x=0;
			if (game->snake[i].y<0) game->snake[i].y=WINDOW_HEIGHT-1;
			if (game->snake[i].y>WINDOW_HEIGHT-1) game->snake[i].y=0;
		} else {
			old_snake = game->snake[i]; 
			game->snake[i] = new_snake;
			new_snake = old_snake;
		}
	}
	
	if (game->extend_left) {
		addSnake(game, old_snake.x, old_snake.y);
		--game->extend_left;
	}

	// drawing
	draw(game);
	SDL_Delay(GAME_SPEED);
}

int main(int argc, char const *argv[]) {
	Game game;
	init(&game);

	while (game.state>0) {
		update(&game);
	}

	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}