#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "obj.h"

#define TITLE "Maki's Obj Test"
#define WIDTH 1280
#define HEIGHT 720

#define OBJ_PATH "obj/spaceship.obj"

#define CAMERA_MOVE_SPEED 0.06
#define CAMERA_ROTATE_SPEED 2

#define MATH_PI 3.14159

typedef enum {false, true} bool;

enum {
	keyW, keyA, keyS, keyD, 
	keyI, keyJ, keyK, keyL,
	keySpace, keyShift
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
	bool key[10];

	Obj* obj;

	int state;
} Program;

// borrowed from OpenGL Utility Library
void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

// borrowed from OpenGL Utility Library
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    float ymax, xmax;
    float temp, temp2, temp3, temp4;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    // ymin = -ymax;
    // xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void setPerspectiveMatrix(float fov, float aspect, float near, float far) {
	// float m[4][4];
	// memset(m, 0, 16*sizeof(float));

	// float angle = (fov/180.0)*MATH_PI;
	// float f = 1.0/tan(angle*0.5);

	// m[0][0] = f/aspect;
	// m[1][1] = f;
	// m[2][2] = (far+near)/(near-far);
	// m[3][2] = -1.0;
	// m[2][3] = (2.0*far*near)/(near-far);

	//float m[16];
	//glhPerspectivef2(&m, 90, WIDTH/HEIGHT, 0.1, 2560);

	//glLoadMatrixf(m);
} 

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

	setPerspectiveMatrix(90, 1, 1, 128);
	glMatrixMode(GL_PROJECTION);

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

void drawObj(Obj* obj) {

	glBegin(GL_TRIANGLES);

	for (int f=0; f<obj->totalFaces; ++f) { //face 
		glColor3f(1.0, 1.0, 1.0);

		for (int v=0; v< 3; ++v) { // vertex
			glVertex3f(
				obj->vertices[obj->faces[f][v]][0],
				obj->vertices[obj->faces[f][v]][1],
				obj->vertices[obj->faces[f][v]][2]
			);
		}
	}

	glEnd();

}

void draw(Program* program) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(
		-program->camera.x,
		-program->camera.y,
		-program->camera.z
	);

<<<<<<< HEAD
	// glRotatef(
	// 	program->camera.rx,
	// 	program->camera.ry,
	// 	program->camera.rz,
	// 0);

	drawObj(program->obj);
=======
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
>>>>>>> refs/remotes/origin/master

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
					case SDLK_SPACE: program->key[keySpace] = true; break;
					case SDLK_LSHIFT: program->key[keyShift] = true; break;
				}
				break;

			case SDL_KEYUP:
				switch(program->event.key.keysym.sym) {
					case SDLK_w: program->key[keyW] = false; break; case SDLK_a: program->key[keyA] = false; break;
					case SDLK_s: program->key[keyS] = false; break; case SDLK_d: program->key[keyD] = false; break;
					case SDLK_i: program->key[keyI] = false; break; case SDLK_j: program->key[keyJ] = false; break;
					case SDLK_k: program->key[keyK] = false; break; case SDLK_l: program->key[keyL] = false; break;
					case SDLK_SPACE: program->key[keySpace] = false; break;
					case SDLK_LSHIFT: program->key[keyShift] = false; break;

					case SDLK_ESCAPE: program->state = 0; break;
				}
				break;

			case SDL_QUIT:
				program->state = 0;
				break;
		}
	}

	// Camera Movement
	if (program->key[keyA]) program->camera.x -= CAMERA_MOVE_SPEED;
	if (program->key[keyD]) program->camera.x += CAMERA_MOVE_SPEED;

	if (program->key[keyShift]) program->camera.y -= CAMERA_MOVE_SPEED;
	if (program->key[keySpace]) program->camera.y += CAMERA_MOVE_SPEED;

	if (program->key[keyW]) program->camera.z -= CAMERA_MOVE_SPEED;
	if (program->key[keyS]) program->camera.z += CAMERA_MOVE_SPEED;

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

<<<<<<< HEAD
	Obj obj;
	if (loadObj(&obj, OBJ_PATH)<0) {
		printf("Failed to load %s\n", OBJ_PATH);
		program.state = -1;
	}
	program.obj = &obj;
=======
	Obj object = loadObj("obj/spaceship.obj");
>>>>>>> refs/remotes/origin/master

	while (program.state > 0) {
		update(&program);
	}

	deinit(&program);
	return 0;
}