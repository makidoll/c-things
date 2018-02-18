#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Maki's Raytracer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_SIZE 1
#define WINDOW_MAP_SIZE 4

#define FOV 90

#define GAME_MOVE_SPEED 8
#define GAME_MOUSE_SPEED 0.6

#define MAX_MAP_SIZE 64
#define MATH_PI 3.14159
#define min(x, y) (((x)<(y))? x: y)

typedef enum {false, true} bool;

typedef struct {
	int x;
	int y;
} Vec2i;

typedef struct {
	float x;
	float y;
} Vec2f;

typedef struct {
	float x;
	float y;
	float a;
	bool keyDown[4];
} Player;

typedef struct {
	int width;
	int height;
	unsigned char data[MAX_MAP_SIZE*MAX_MAP_SIZE];
} Map;

typedef struct {
	int map_x;
	int map_y;
	int player_x;
	int player_y;
} Minimap;

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

	Player player;
	Map map;
	Minimap minimap;

	int state;
	bool locked;

	double dt;
	clock_t dtc;
} Game;

void loadMap(Game* game) {
	// reading map from file
	FILE* file;
	file = fopen("map.txt", "r");

	if (!file) {
		fprintf(stderr, "Couldn't read map.txt!\n");
		game->state = -1;
	}

	char map_width[4] = "\0\0\0\0";
	char map_height[4] = "\0\0\0\0";

	char map_metadata_i = 0; 

	int chr; 
	int pos = 0;
	int line = 0;
	while ((chr=getc(file)) != EOF) {
		if (chr == 10) { // new line
			if (line == 0) {
				// last metadata
				game->map.width = atoi(map_width);
				game->map.height = atoi(map_height);
			}

			pos = 0;
			++line;
			continue;
		}

		if (line == 0) {
			++pos;
			if (chr == 44) {
				++map_metadata_i; // comma
				pos = 0;
			}
			if (map_metadata_i == 0) { map_width[pos-1] = chr; continue; }
			if (map_metadata_i == 1) { map_height[pos-1] = chr; continue; }
		}
		
		if (chr == 80) { // P
			game->player.x = pos;
			game->player.y = line-1;
			game->map.data[pos+((line-1)*game->map.width)] = 0;
			++pos;
			continue;
		}

		game->map.data[pos+((line-1)*game->map.width)] = chr-48;
		++pos;
	}

	for (int i=0; i<game->map.height*game->map.width; ++i) {
		if (i%game->map.width == 0) printf("\n");
		printf("%d", game->map.data[i]);
	}

	printf("\nMap loaded! (%d,%d)\n", game->map.width, game->map.height);
}

void initMinimap(Game* game) {
	game->minimap.map_x = 0;
	game->minimap.map_y =
		WINDOW_HEIGHT*WINDOW_SIZE
		-game->map.height*WINDOW_MAP_SIZE;
}

void initGame(Game* game) {
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
		game->dt = 0;

		game->locked = true;
		SDL_SetRelativeMouseMode(SDL_TRUE);

		loadMap(game);
		initMinimap(game);

		memset(game->player.keyDown, 0, sizeof(bool)*4);
		game->player.a = MATH_PI*0.65;

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

void Vec2f_normalize(Vec2f* a) {
	float l = sqrt(a->x*a->x + a->y*a->y);
	a->x = a->x/l; 
	a->y = a->y/l; 
}

void Vec2f_multiply(Vec2f* a, float b) {
	a->x *= b; 
	a->y *= b; 
}

void Vec2f_add(Vec2f* a, float b) {
	a->x += b; 
	a->y += b; 
}

void drawGradientV(
	SDL_Renderer* renderer, int x, int y, int w, int h, 
	int r0, int g0, int b0, int r1, int g1, int b1) {

	int dr = r0-r1;
	int dg = g0-g1;
	int db = b0-b1;
	for (int cy=0; cy<h; ++cy) {
		SDL_SetRenderDrawColor(renderer, 
			r0-dr*cy/h,
			g0-dg*cy/h,
			b0-db*cy/h,
		255);
		drawRect(renderer, x, cy+y, w, 1);

	}

}

void draw(Game* game) {
	SDL_SetRenderDrawColor(game->renderer, 0,0,0,255);
	SDL_RenderClear(game->renderer);

	// background
	drawGradientV(game->renderer, 0, 0,
		WINDOW_WIDTH*WINDOW_SIZE,
		WINDOW_HEIGHT*WINDOW_SIZE/2,
		128, 128, 128, 0, 0, 0
	);

	drawGradientV(game->renderer, 0, WINDOW_HEIGHT*WINDOW_SIZE/2,
		WINDOW_WIDTH*WINDOW_SIZE,
		WINDOW_HEIGHT*WINDOW_SIZE/2,
		0, 0, 0, 128, 128, 128
	);
	// SDL_SetRenderDrawColor(game->renderer, 0,0,255,255);
	// drawRect(game->renderer,
	// 	0,0,WINDOW_WIDTH*WINDOW_SIZE,
	// 	(WINDOW_HEIGHT*WINDOW_SIZE)/2
	// );

	// shoot rays
	for (int screen_x=0; screen_x<WINDOW_WIDTH; ++screen_x) {

		//float sl = ((float)screen_x/(WINDOW_WIDTH/2))-1; // screen length: -1, 0, 1
		float ra = (screen_x-WINDOW_WIDTH/2)*(MATH_PI/WINDOW_WIDTH*FOV/180); // ray angle
		//float rl = 1/cos(ra); // ray length 


		Vec2f rd; // ray direcitons 
		rd.x = sin(game->player.a-ra); // *rl
		rd.y = cos(game->player.a-ra); // *rl


		Vec2f rs; // ray start
		rs.x = game->player.x; //-sin(game->player.a)*2;
		rs.y = game->player.y; //-cos(game->player.a)*2;


		float d = 0; // distance
		Vec2i rp; // ray position
		while (d<256) {
			rp.x = rs.x+(rd.x*d);
			rp.y = rs.y+(rd.y*d);

			if (game->map.data[rp.x+rp.y*game->map.width]>0) break;
			d+=0.02;
		}

		int d_pos = (WINDOW_HEIGHT*WINDOW_SIZE)/2-(WINDOW_HEIGHT*WINDOW_SIZE)/d;

		int c = d*10;
		if (c>255) c = 255;

		SDL_SetRenderDrawColor(game->renderer, 255-c, 255-c, 255-c, 255);
		drawRect(game->renderer,
			screen_x*WINDOW_SIZE, d_pos,
			WINDOW_SIZE,
			(WINDOW_HEIGHT*WINDOW_SIZE)-d_pos*2
		);

		//float minimap_rs_x = game->minimap.map_x+rs.x*WINDOW_MAP_SIZE+WINDOW_MAP_SIZE/2;
		//float minimap_rs_y = game->minimap.map_y+rs.y*WINDOW_MAP_SIZE+WINDOW_MAP_SIZE/2;

		// if (screen_x%((WINDOW_WIDTH*WINDOW_SIZE)/32) == 0) {	
		// 	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		// 	SDL_RenderDrawLine(game->renderer,
		// 		minimap_rs_x, minimap_rs_y,
		// 		minimap_rs_x+rd.x*16,
		// 		minimap_rs_y+rd.y*16);
		// }
	}

	// draw map
	int map_x = 0; int map_y = -1;
	for (int i=0; i<game->map.height*game->map.width; ++i) {
		if (i%game->map.width == 0) {
			map_x = 0;
			++map_y;
		}

		switch (game->map.data[i]) {
			case 1: SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); break;
			default: SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255); break;
		}

		drawRect(game->renderer,
			game->minimap.map_x+map_x*WINDOW_MAP_SIZE+WINDOW_MAP_SIZE,
			game->minimap.map_y+map_y*WINDOW_MAP_SIZE-WINDOW_MAP_SIZE,
			WINDOW_MAP_SIZE, WINDOW_MAP_SIZE
		);

		++map_x;
	}

	// draw player on map
	game->minimap.player_x =
		game->minimap.map_x
			+game->player.x*WINDOW_MAP_SIZE
			+WINDOW_MAP_SIZE/2;

	game->minimap.player_y =
		game->minimap.map_y
			+game->player.y*WINDOW_MAP_SIZE
			+WINDOW_MAP_SIZE/2;

	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
	drawRect(game->renderer,
		game->minimap.map_x+game->player.x*WINDOW_MAP_SIZE+WINDOW_MAP_SIZE,
		game->minimap.map_y+game->player.y*WINDOW_MAP_SIZE-WINDOW_MAP_SIZE,
		WINDOW_MAP_SIZE, WINDOW_MAP_SIZE
	);

	SDL_RenderDrawLine(game->renderer,
		game->minimap.player_x+WINDOW_MAP_SIZE,
		game->minimap.player_y-WINDOW_MAP_SIZE,
		game->minimap.player_x+sin(game->player.a)*WINDOW_MAP_SIZE*2+WINDOW_MAP_SIZE,
		game->minimap.player_y+cos(game->player.a)*WINDOW_MAP_SIZE*2-WINDOW_MAP_SIZE
	);

	SDL_RenderPresent(game->renderer);
}

void update(Game* game) {
	game->dtc = clock();

	// events
	while(SDL_PollEvent(&game->event)) {
		switch(game->event.type) {
			case SDL_KEYDOWN:
				if (!game->locked) break;
				switch(game->event.key.keysym.sym) {
					case SDLK_w: game->player.keyDown[0] = true; break;
					case SDLK_a: game->player.keyDown[1] = true; break;
					case SDLK_s: game->player.keyDown[2] = true; break;
					case SDLK_d: game->player.keyDown[3] = true; break;
					//case SDLK_d: game->player.keyDown[3] = true; break;
				}
				break;

			case SDL_KEYUP:
				switch(game->event.key.keysym.sym) {
					case SDLK_w: game->player.keyDown[0] = false; break;
					case SDLK_a: game->player.keyDown[1] = false; break;
					case SDLK_s: game->player.keyDown[2] = false; break;
					case SDLK_d: game->player.keyDown[3] = false; break;
					case SDLK_ESCAPE: 
						if (game->locked) {
							game->locked = false;
							SDL_SetRelativeMouseMode(SDL_FALSE);
							break;
						}
						game->state = 0; break;
				}
				break;

			case SDL_MOUSEMOTION:
				if (!game->locked) break;
				game->player.a -= game->event.motion.xrel*game->dt*GAME_MOUSE_SPEED*WINDOW_SIZE;
				if (game->player.a>MATH_PI*2) game->player.a = 0;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (!game->locked) { 
					game->locked = true;
					SDL_SetRelativeMouseMode(SDL_TRUE);
					break;
				}
				break;

			case SDL_QUIT: game->state = 0; break;
		}
	}

	// player movement
	if (game->player.keyDown[0]) {
		game->player.x += sin(game->player.a)*game->dt*GAME_MOVE_SPEED;
		game->player.y += cos(game->player.a)*game->dt*GAME_MOVE_SPEED;
	}

	if (game->player.keyDown[1]) {
		game->player.x += sin(game->player.a+MATH_PI/2)*game->dt*GAME_MOVE_SPEED;
		game->player.y += cos(game->player.a+MATH_PI/2)*game->dt*GAME_MOVE_SPEED;
		//game->player.a += game->dt*GAME_MOUSE_SPEED;
	}

	if (game->player.keyDown[2]) {
		game->player.x -= sin(game->player.a)*game->dt*GAME_MOVE_SPEED;
		game->player.y -= cos(game->player.a)*game->dt*GAME_MOVE_SPEED;
	}
	
	if (game->player.keyDown[3]) {
		game->player.x -= sin(game->player.a+MATH_PI/2)*game->dt*GAME_MOVE_SPEED;
		game->player.y -= cos(game->player.a+MATH_PI/2)*game->dt*GAME_MOVE_SPEED;
		//game->player.a -= game->dt*GAME_MOUSE_SPEED;
	}

	draw(game);

	#ifdef _WIN32
		game->dt = ((float)(clock()-game->dtc)/1000000.0F)*1000; 
	#else
		game->dt = ((float)(clock()-game->dtc)/1000000000.0F)*1000; 
	#endif
}

int main (int argc, char* argv[]) {
	
	Game game;
	initGame(&game);

	while (game.state>0) {
		update(&game);
	}

	printf("Closing game.\n");

	SDL_DestroyWindow(game.window);
	SDL_Quit();
	return 0;
}