#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Maki's 3D Raytracer"
#define WINDOW_WIDTH 128
#define WINDOW_HEIGHT 128
#define WINDOW_SIZE 4

#define PLAYER_FOV 90
#define PLAYER_MOVE_SPEED 0.1

#define MATH_PI 3.14159
#define MAX_MAP_SIZE 64

typedef enum {false, true} bool;

typedef enum {
	BUTTON_W, BUTTON_A,
	BUTTON_S, BUTTON_D,

	BUTTON_I, BUTTON_J,
	BUTTON_K, BUTTON_L,

	BUTTON_E, BUTTON_Q
} BUTTON;

typedef struct {
	float x; float y; float z;
} Vec3f;

typedef struct {
	int x; int y; int z;
} Vec3i;

typedef struct {
	float x; float y; float z;
	float rx;
} Player;

typedef struct {
	char data[MAX_MAP_SIZE*MAX_MAP_SIZE*MAX_MAP_SIZE];
	unsigned int width; // x
	unsigned int height; // y
	unsigned int depth; // z
} Map;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	Player player;
	BUTTON key[10];
	//Map map;

	int state;
} Game;


char getMap(Map* map, int x, int y, int z) {
	return map->data[
		x*map->height*map->depth+
		y*map->height+z
	];
}

void init(Game* game) {
	if (SDL_Init(SDL_INIT_EVERYTHING)==0) {
		// sdl
		game->window = SDL_CreateWindow(
			WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH*WINDOW_SIZE, WINDOW_HEIGHT*WINDOW_SIZE,
			SDL_WINDOW_SHOWN
		);

		game->renderer = SDL_CreateRenderer(
			game->window, -1, 
			SDL_RENDERER_ACCELERATED
		);

		// player
		game->player.x = 0;
		game->player.y = 1;
		game->player.z = 0;

		game->player.rx = 0;

		// map
		// memset(game->map.data, 0, MAX_MAP_SIZE*MAX_MAP_SIZE*MAX_MAP_SIZE*sizeof(char));
		// game->map.width = 32;
		// game->map.height = 32;
		// game->map.depth = 32;
		// game->map.data[0][0][4] = 1;

		// other
		game->state = 1;
		memset(game->key, 0, 6*sizeof(bool));
	} else {
		game->state = -1;
	}
}

void drawPixel(SDL_Renderer* renderer, int x, int y) {
	SDL_Rect rect;
	rect.x = x*WINDOW_SIZE;
	rect.y = y*WINDOW_SIZE;
	rect.w = WINDOW_SIZE;
	rect.h = WINDOW_SIZE;
	SDL_RenderFillRect(renderer, &rect);
}

bool cFloor(Vec3f rp, float y) {
	if (rp.y<y) return true;
	return false;
}

bool cCube(Vec3f rp, float x, float y, float z, float s) {
	if (
		(rp.x>x-s/2 && rp.x<x+s/2) &&
		(rp.y>y-s/2 && rp.y<y+s/2) &&
		(rp.z>z-s/2 && rp.z<z+s/2) 
	) {
		return true;
	} else {
		return false;
	}
}

void draw(Game* game) {
	// shoot rays
	for (int screen_y=0; screen_y<WINDOW_HEIGHT; ++screen_y) {
		for (int screen_x=0; screen_x<WINDOW_WIDTH; ++screen_x) {
			// ray angle
			float ra_x = (screen_x-WINDOW_WIDTH/2)*(MATH_PI/WINDOW_WIDTH*PLAYER_FOV/180); // ray angle x
			float ra_y = -(screen_y-WINDOW_HEIGHT/2)*(MATH_PI/WINDOW_HEIGHT*PLAYER_FOV/180); // ray angle y

			Vec3f rd; // ray direction
			rd.x = ra_x;
			rd.y = ra_y;
			rd.z = 1;

			Vec3f ro; // ray origin
			ro.x = game->player.x;
			ro.y = game->player.y;
			ro.z = game->player.z;

			float d = 0; // distance
			Vec3f rp; // ray position

			//int mat = 0;
			while (d<4) {
				rp.x = ro.x+(rd.x*d);
				rp.y = ro.y+(rd.y*d);
				rp.z = ro.z+(rd.z*d);

				// // collide outside
				// if (
				// 	(abs(rp.x)>game->map.width/2) ||
				// 	(abs(rp.y)>game->map.height/2) ||
				// 	(abs(rp.z)>game->map.depth/2)
				// ) {
				// 	break;
				// }

				// // collide with voxel
				// if (game->map.data[rp.x][rp.y][rp.z]>0) {
				// 	//mat = game->map.data[rp.x][rp.y][rp.z];
				// 	break;
				// }

				if (cFloor(rp, -0.5)) break;
				if (cCube(rp, 0, 0, 3, 1)) break;

				d += 0.1;
			}

			int c = 255-d*32;
			if (c>255) c=255;
			if (c<0) c=0;
			SDL_SetRenderDrawColor(game->renderer, c, c, c, 255);
			drawPixel(game->renderer, screen_x, screen_y);
		}
	}

	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	// events
	while (SDL_PollEvent(&game->event)) {
		switch (game->event.type) {
			case SDL_KEYDOWN: 
				switch(game->event.key.keysym.sym) {
					case SDLK_w: game->key[BUTTON_W] = true; break;
					case SDLK_a: game->key[BUTTON_A] = true; break;
					case SDLK_s: game->key[BUTTON_S] = true; break;
					case SDLK_d: game->key[BUTTON_D] = true; break;

					case SDLK_i: game->key[BUTTON_I] = true; break;
					case SDLK_j: game->key[BUTTON_J] = true; break;
					case SDLK_k: game->key[BUTTON_K] = true; break;
					case SDLK_l: game->key[BUTTON_L] = true; break;

					case SDLK_e: game->key[BUTTON_E] = true; break;
					case SDLK_q: game->key[BUTTON_Q] = true; break;
				}
				break;

			case SDL_KEYUP: 
				switch(game->event.key.keysym.sym) {
					case SDLK_w: game->key[BUTTON_W] = false; break;
					case SDLK_a: game->key[BUTTON_A] = false; break;
					case SDLK_s: game->key[BUTTON_S] = false; break;
					case SDLK_d: game->key[BUTTON_D] = false; break;

					case SDLK_i: game->key[BUTTON_I] = false; break;
					case SDLK_j: game->key[BUTTON_J] = false; break;
					case SDLK_k: game->key[BUTTON_K] = false; break;
					case SDLK_l: game->key[BUTTON_L] = false; break;

					case SDLK_e: game->key[BUTTON_E] = false; break;
					case SDLK_q: game->key[BUTTON_Q] = false; break;
				}
				break;
			
			case SDL_QUIT: game->state = 0; break;
		}
	}

	// player movement
	if (game->key[BUTTON_W]) { game->player.z+=PLAYER_MOVE_SPEED; } 
	if (game->key[BUTTON_A]) { game->player.x-=PLAYER_MOVE_SPEED; }
	if (game->key[BUTTON_S]) { game->player.z-=PLAYER_MOVE_SPEED; }
	if (game->key[BUTTON_D]) { game->player.x+=PLAYER_MOVE_SPEED; }



	if (game->key[BUTTON_E]) { game->player.y+=PLAYER_MOVE_SPEED; }
	if (game->key[BUTTON_Q]) { game->player.y-=PLAYER_MOVE_SPEED; }

	draw(game);
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