#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>

typedef struct { float x; float y; float z; } Vec3f;
typedef struct { int x; int y; int z; } Vec3i;

SDL_Point project(Vec3f v) {
	SDL_Point p = {
		(int)(256 + v.x*32),
		(int)(256 - v.y*32)
	};
	return p;
}

int main (int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"Maki's 3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		512, 512, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);

	Vec3f pos = {0, 0, 0};
	Vec3f rot = {0, 0, 0};

	#include "obj.h" // import verts & faces
	int cFaces = sizeof(faces)/sizeof(Vec3i);

	SDL_Point lines[cFaces*3*2]; // faces * (tris are 3) * (2 points is 1 line)

	int state = 1;
	SDL_Event event;
	while (state) {
		// update
		int key = 0;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: state = 0; break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_w: key = 1; break;
						case SDLK_a: key = 2; break;
						case SDLK_s: key = 3; break;
						case SDLK_d: key = 4; break;
					}
				break;
			}
		}

		// drawing
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);



		// for (int iFaces=0; iFaces<cFaces; ++iFaces) {
		// 	SDL_Point c0 = project(verts[faces[iFaces].x]); // xyz doesnt make sense
		// 	SDL_Point c1 = project(verts[faces[iFaces].y]); // here but whatever...
		// 	SDL_Point c2 = project(verts[faces[iFaces].z]);

		// 	lines[iFaces*6    ] = c0; lines[iFaces*6 + 1] = c1;
		// 	lines[iFaces*6 + 2] = c1; lines[iFaces*6 + 3] = c2;
		// 	lines[iFaces*6 + 4] = c2; lines[iFaces*6 + 5] = c0;
		// }

		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderDrawLines(renderer, lines, cFaces*3*2);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}