#include <stdio.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "obj.h"

#define TITLE "Maki's Obj Test"
#define WIDTH 480
#define HEIGHT 480

#define CAMERA_MOVE_SPEED 0.06
#define CAMERA_ROTATE_SPEED 2

typedef enum {false, true} bool;

enum {
	keyW, keyA, keyS, keyD, 
	keyI, keyJ, keyK, keyL 
};

typedef struct {
	float x; float y; float z;
	float rx; float ry; float rz;
} Camera;

typedef struct {
	SDL_Window* window;
	SDL_GLContext* context;
	SDL_Event event;

	Camera camera;
	bool key[8];

	int state;
} Program;

void resetCamera(Camera* camera) {
	camera->x  = 0; camera->y  = 0; camera->z  = 0;
	camera->rx = 0; camera->ry = 0; camera->rz = 0;
}

int init(Program* program) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Failed to init SDL\n");
		return -1;
	}

	program->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!program->window) {
		fprintf(stderr, "Failed to make window\n");
		return -1;
	}

	program->context = SDL_GL_CreateContext(program->window);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	#ifndef __APPLE__
		glewExperimental = GL_TRUE;
		glewInit();
	#endif

	resetCamera(&program->camera);

	memset(program->key, false, sizeof(program->key)*sizeof(bool));

	program->state = 1;
	return 1;
}

void deinit(Program* program) {
	SDL_GL_DeleteContext(program->context);
	SDL_DestroyWindow(program->window);
	SDL_Quit();
}

void draw(Program* program) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(
		program->camera.x,
		program->camera.y,
		program->camera.z
	);

	glRotatef(
		program->camera.rx,
		program->camera.ry,
		program->camera.rz,
	0);

	glBegin(GL_POLYGON);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.5);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(-0.5, 0.5, 0.5);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.5, 0.5, 0.5);

	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.5, -0.5, 0.5);

	glEnd();

	glFlush();
	SDL_GL_SwapWindow(program->window);
}

void update(Program* program) {
	while(SDL_PollEvent(&program->event)) {
		// Events
		switch(program->event.type) {
			case SDL_KEYDOWN:
				switch(program->event.key.keysym.sym) {
					case SDLK_w: program->key[keyW] = true; break; case SDLK_a: program->key[keyA] = true; break;
					case SDLK_s: program->key[keyS] = true; break; case SDLK_d: program->key[keyD] = true; break;
					case SDLK_i: program->key[keyI] = true; break; case SDLK_j: program->key[keyJ] = true; break;
					case SDLK_k: program->key[keyK] = true; break; case SDLK_l: program->key[keyL] = true; break;
				}
				break;

			case SDL_KEYUP:
				switch(program->event.key.keysym.sym) {
					case SDLK_w: program->key[keyW] = false; break; case SDLK_a: program->key[keyA] = false; break;
					case SDLK_s: program->key[keyS] = false; break; case SDLK_d: program->key[keyD] = false; break;
					case SDLK_i: program->key[keyI] = false; break; case SDLK_j: program->key[keyJ] = false; break;
					case SDLK_k: program->key[keyK] = false; break; case SDLK_l: program->key[keyL] = false; break;
					case SDLK_ESCAPE: program->state = 0; break;
				}
				break;

			case SDL_QUIT:
				program->state = 0;
				break;
		}
	}

	// Camera Movement
	if (program->key[keyW]) program->camera.y -= CAMERA_MOVE_SPEED;
	if (program->key[keyA]) program->camera.x += CAMERA_MOVE_SPEED;
	if (program->key[keyS]) program->camera.y += CAMERA_MOVE_SPEED;
	if (program->key[keyD]) program->camera.x -= CAMERA_MOVE_SPEED;

	// Camera Rotation
	if (program->key[keyI]) program->camera.ry -= CAMERA_ROTATE_SPEED;
	if (program->key[keyJ]) program->camera.rx += CAMERA_ROTATE_SPEED;
	if (program->key[keyK]) program->camera.ry += CAMERA_ROTATE_SPEED;
	if (program->key[keyL]) program->camera.rx -= CAMERA_ROTATE_SPEED;

	// printf("Camera: %2.2f, %2.2f, %2.2f (%2.2f, %2.2f, %2.2f)\n",
	// 	program->camera.x,  program->camera.y,  program->camera.z,
	// 	program->camera.rx, program->camera.ry, program->camera.rz
	// );

	// Drawing
	draw(program);
}

int main(int argc, char const* argv[]) {
	// if (argc <= 1) {
	// 	printf("usage: (obj)\n");
	// 	return 0;
	// }

	// printf("Loading %s...\n", argv[1]);

	Program program;
	if (init(&program) < 0) return 0;

	Obj object = loadObj("obj/spaceship.obj");

	while (program.state > 0) {
		update(&program);
	}

	deinit(&program);
	return 0;
}