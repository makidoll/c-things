#include <ncurses.h>
#include <stdio.h>

#define SIZE 24 

#define MIN(x, y) (x<y)?x:y

typedef struct {
	WINDOW* win_game;
	WINDOW* win_score;
	int screen[SIZE][SIZE];

	int score;
	int state; // off, game, dead 

	int dir; // up, down, left, right
	int x; int y;
} Game;

// Game Functions

void addScore(Game* game, int score) {
	game->score += score;
	mvwprintw(game->win_score, 1, 2, "Score: %d", game->score);
	wrefresh(game->win_score);
}

void clearScreen(Game* game) {
	for (int x=0; x<SIZE; ++x) {
		for (int y=0; y<SIZE; ++y) {
			game->screen[x][y] = 0;
		}	
	}
}

// Main Functions

void init(Game* game) {
	initscr(); start_color(); cbreak(); refresh();

	game->score = 0; 
	game->state = 1;
	game->x = SIZE/2;
	game->y = SIZE/2;
	game->dir = 0; // up
	clearScreen(game);

	// Ncurses
	game->win_game = newwin(SIZE+2, SIZE*2+2,
		LINES/2-SIZE/2-2, COLS/2-SIZE-1);

	game->win_score = newwin(3, SIZE,
		LINES/2-SIZE/2+SIZE, COLS/2-SIZE+2);

	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);

	box(game->win_game, 0, 0);
	box(game->win_score, 0, 0);

	wattron(game->win_score, COLOR_PAIR(1));
	mvwprintw(game->win_score, 0, 1, "Snake by Maki");
	wattroff(game->win_score, COLOR_PAIR(1));

	wrefresh(game->win_game);
	addScore(game, 0);
}

void draw(Game* game) {
	wclear(game->win_game);
	box(game->win_game, 0, 0);

	wattron(game->win_game, COLOR_PAIR(2));
	for (int x=0; x<SIZE; ++x) {
		for (int y=0; y<SIZE; ++y) {
			if (game->screen[x][y]) mvwprintw(game->win_game, y+1, x*2+1, "  ");
		}	
	}
	wattroff(game->win_game, COLOR_PAIR(2));
	wrefresh(game->win_game);
}

void update(Game* game) {
	
	// Game
	switch(game->dir) {
		case 0: game->y--;
		case 1: game->y++;
		case 2: game->x--;
		case 3: game->x++;
	}

	// Drawing
	clearScreen(game);
	game->screen[game->x][game->y] = 1;
	draw(game);

	// Input
	// switch (getch()) {
	// 	default:
	// 		game->y-=1;
	// 		addScore(game, 1);
	// 		break; 
	// }
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
