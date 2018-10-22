#include <stdio.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define PI 3.14159265

typedef struct { float x; float y; float z; } Vec3f;
typedef struct { int x; int y; int z; } Vec3i;
typedef struct { float x; float y; } Vec2f;

SDL_Point project(Vec3f v) {
	float z = (v.z>0)? 256/v.z: 0;
	SDL_Point p = {
		(int)((256 + v.x*z)),
		(int)((256 - v.y*z))
	};
	return p;
}

void rotateXZ(Vec3f* v, float r) {
	float x = v->x; float z = v->z;
	v->x = x*cos(r) - z*sin(r);
	v->z = z*cos(r) + x*sin(r);
}

void rotateYZ(Vec3f* v, float r) {
	float y = v->y; float z = v->z;
	v->y = y*cos(r) - z*sin(r);
	v->z = z*cos(r) + y*sin(r);
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow(
		"Maki's 3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		512, 512, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(
		window, -1, SDL_RENDERER_ACCELERATED);

	Vec3f pos = {0,0,0}; float posSpeed = 12;
	Vec2f rot = {0,0}; float rotSpeed = 6; // only x,y

	#include "obj.h" // import verts & faces

	int cFaces = sizeof(faces)/sizeof(Vec3i); // count faces
	int cVerts = sizeof(verts)/sizeof(Vec3f); // count verts

	Vec3f tVerts[cVerts]; // transformed verts
	SDL_Point lines[cFaces*3*2]; // faces * (tris are 3) * (2 points is 1 line)

	char keyDown[2] = {0}; // ws, ad, eq

	int state = 1;
	SDL_Event event;

	//double time = 0;
	double dt = 0;
	clock_t dtc;

	int locked = 1;

	while (state) {
		// update
		dtc = clock();

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: state = 0; break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_w: keyDown[0]= 1; break;
						case SDLK_s: keyDown[0]=-1; break;
						case SDLK_a: keyDown[1]= 1; break;
						case SDLK_d: keyDown[1]=-1; break;
						case SDLK_e: keyDown[2]= 1; break;
						case SDLK_q: keyDown[2]=-1; break;
					}
				break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_w: keyDown[0]=0; break;
						case SDLK_s: keyDown[0]=0; break;
						case SDLK_a: keyDown[1]=0; break;
						case SDLK_d: keyDown[1]=0; break;
						case SDLK_e: keyDown[2]=0; break;
						case SDLK_q: keyDown[2]=0; break;

						case SDLK_ESCAPE:
							locked = (locked)? 0: 1;
							break;
					}
				break;
				case SDL_MOUSEMOTION:
					if (!locked) break;
					rot.x += event.motion.xrel*rotSpeed*dt;
					rot.y -= event.motion.yrel*rotSpeed*dt;
				break;
			}
		}

		if (locked) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
		} else {
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}

		if (locked) {		
			if (keyDown[0]!=0) {
				pos.x -= cos(rot.x+PI/2)*
					posSpeed*keyDown[0]*dt;
				pos.z += sin(rot.x+PI/2)*
					posSpeed*keyDown[0]*dt;
			}

			if (keyDown[1]!=0) {
				pos.x -= cos(rot.x)*
					posSpeed*keyDown[1]*dt;
				pos.z += sin(rot.x)*
					posSpeed*keyDown[1]*dt;
			}
	
			if (keyDown[2]!=0) {
				pos.y += posSpeed*keyDown[2]*dt;
			}
		}

		// drawing
		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);

		// transform verts
		for (int iVerts=0; iVerts<cVerts; ++iVerts) {
			tVerts[iVerts].x = verts[iVerts].x-pos.x;
			tVerts[iVerts].y = verts[iVerts].y-pos.y;
			tVerts[iVerts].z = verts[iVerts].z-pos.z;
			rotateXZ(&tVerts[iVerts], rot.x);
			rotateYZ(&tVerts[iVerts], rot.y);
		}

		// draw faces
		for (int iFaces=0; iFaces<cFaces; ++iFaces) {
			SDL_Point c0 = project(tVerts[faces[iFaces].x]); // xyz doesnt make sense
			SDL_Point c1 = project(tVerts[faces[iFaces].y]); // here but whatever...
			SDL_Point c2 = project(tVerts[faces[iFaces].z]);

			lines[iFaces*6    ] = c0; lines[iFaces*6 + 1] = c1;
			lines[iFaces*6 + 2] = c1; lines[iFaces*6 + 3] = c2;
			lines[iFaces*6 + 4] = c2; lines[iFaces*6 + 5] = c0;
		}

		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderDrawLines(renderer, lines, cFaces*3*2);

		SDL_RenderPresent(renderer);

		// delta time
		#ifdef _WIN32
			dt = ((float)(clock()-dtc)/1000000.0F)*1000; 
		#else
			dt = ((float)(clock()-dtc)/1000000000.0F)*1000; 
		#endif
		//time += dt;
	}

	// closing
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}