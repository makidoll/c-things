#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define TITLE "Shader Test"
#define WIDTH 512
#define HEIGHT 512

typedef struct {
	SDL_Window* window;
	SDL_GLContext* context;
	SDL_Event event;

	int state;
} Game;

void initGame(Game* game) {
	SDL_Init(SDL_INIT_EVERYTHING);

	game->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!game->window) {
		fprintf(stderr, "Failed to make window\n");
		exit(0);
	}

	game->context = SDL_GL_CreateContext(game->window);
	SDL_GL_SetSwapInterval(1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glewExperimental = GL_TRUE;
	glewInit();

	game->state = 1;
}

void deinitGame(Game* game) {
	SDL_GL_DeleteContext(game->context);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

void drawGame(Game* game) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(game->window);
}  

void updateGame(Game* game) {
	while(SDL_PollEvent(&game->event)) {
		// Drawing
		drawGame(game);

		// Events
		switch(game->event.type) {
			case(SDL_QUIT):
				game->state = 0;
				break;
		}

		// Keys
		switch(game->event.key.keysym.sym) {
			case SDLK_ESCAPE:
				game->state = 0;
				break;
		}
	}
}

int main(int argc, char* argv[]) {
	Game game;
	initGame(&game);

	// Our vector array contains for vectors, each with 3 points ( x, y, z )
	const uint32_t points = 4;
	const uint32_t floatsPerPoint = 3;
	const GLfloat square[points][floatsPerPoint] =	{
		{ -0.5,  0.5,  0.5 }, // Top left
		{  0.5,  0.5,  0.5 }, // Top right
		{  0.5, -0.5,  0.5 }, // Bottom right 
		{ -0.5, -0.5,  0.5 }, // Bottom left
}	;

	const uint32_t countVBOs = 1;
	GLuint vbo[countVBOs];
	glGenBuffers(countVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	uint32_t sizeInBytes =  ( points * floatsPerPoint) * sizeof(GLfloat);
	glBufferData( GL_ARRAY_BUFFER,sizeInBytes, square, GL_STATIC_DRAW );

	while (game.state>0) {
		updateGame(&game);
	}
	deinitGame(&game);
	return 0;
}