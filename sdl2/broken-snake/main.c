#include "game.h"

int main(int argc, char* argv[]) {
	Game game;
	init(&game);
	while (game.state>0) update(&game);
	deinit(&game);
	return 0;
}