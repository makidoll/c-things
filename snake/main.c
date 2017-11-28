#include <ncurses.h>
#include <stdio.h>

#define WIDTH 48
#define HEIGHT 24 

#define MIN(x, y) (x<y)?x:y

typedef struct {
	WINDOW *win_game;
	WINDOW *win_score;

	int score;
	int state; // 0: off, 1: game, 2: dead 

	int x; int y;
} Game;

// Game Functions

void addScore(Game *game, int score) {
	game->score += score;
	mvwprintw(game->win_score, 1, 2, "Score: %d", game->score);
	wrefresh(game->win_score);
}

// Main Functions

void init(Game *game) {
	initscr(); noecho(); cbreak(); refresh();

	int SIZE = MIN(LINES, COLS/2);

	game->score = 0; 
	game->state = 1;
	game->x = SIZE/2;
	game->y = SIZE/2;

	game->win_game = newwin(SIZE-3, SIZE*2,
		LINES/2-SIZE/2, COLS/2-SIZE);

	game->win_score = newwin(3, SIZE,
		LINES/2-SIZE/2+SIZE-3, COLS/2-SIZE+2);

	box(game->win_game, 0, 0);
	box(game->win_score, 0, 0);

	mvwprintw(game->win_score, 0, 1, "Snake by Maki");

	wrefresh(game->win_game);
	addScore(game, 0);
}

void update(Game *game) {
	// Drawing
	mvwprintw(game->win_game, game->y, game->x*2, "@");
	wmove(game->win_game, 0, 0);
	wrefresh(game->win_game);

	// Input
	switch(getch()) {
		default:
			game->y-=1;
			addScore(game, 1);
			break; 
	}
}

int main() {
	Game game;
	init(&game);
	
	while(game.state > 0) {
		update(&game);
	}
	
	endwin();
	return 0;
}
