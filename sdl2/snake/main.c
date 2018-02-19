#include <SDL2/SDL.h>

typedef struct {

	unsigned char dir; // w,a,s,d 

} Game;

void init(Game* game) {

}

void update(Game* game) {
	
}

int main(int argc, char const *argv[]) {
	Game game;
	init(&game);

	while (game.state>0) {
		update(&game);
	}

	SDL_DestroyWindow(game.window);
	SDL_Quit()
	return 0;
}