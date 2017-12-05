#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

#define WIDTH 48
#define HEIGHT 32
#define UPDATE 1000/24

typedef struct {
	WINDOW *window;
	int cells[HEIGHT][WIDTH];
	int pre_cells[HEIGHT][WIDTH];
	int frame;
} Game;

void drawCells(Game* game) {
	wclear(game->window);
	box(game->window, 0, 0);

	wattron(game->window, COLOR_PAIR(1));
	mvwprintw(game->window, HEIGHT+1, 2, "Game of Life by Maki");
	mvwprintw(game->window, HEIGHT+1, 24, "Frame: %d", game->frame);
	wattroff(game->window, COLOR_PAIR(1));

	wattron(game->window, COLOR_PAIR(2));
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			switch (game->cells[y][x]) {
				case (1):
					mvwprintw(game->window, y+1, x*2+1, "  ");
					break;
				default:
					break;
			}
		}
	}
	wattroff(game->window, COLOR_PAIR(2));
	wrefresh(game->window);
}

void addRpentomino(Game* game, int y, int x) {
	game->cells[y][x+1] = 1;
	game->cells[y][x+2] = 1;
	game->cells[y+1][x] = 1;
	game->cells[y+1][x+1] = 1;
	game->cells[y+2][x+1] = 1;
}

void addGlider(Game* game, int y, int x) {
	game->cells[y][x+1] = 1;
	game->cells[y+1][x+2] = 1;
	game->cells[y+2][x] = 1;
	game->cells[y+2][x+1] = 1;
	game->cells[y+2][x+2] = 1;
}

int getPreNeighbours(Game* game, int yy, int xx) {
	int neighbours = 0;

	for (int y=-1; y<2; y++) {
		for (int x=-1; x<2; x++) {
			if (
				(y != 0 || x != 0) &&
				(xx+x > -1 && xx+x < WIDTH) &&
				(yy+y > -1 && yy+y < HEIGHT)
			) {
				if (game->pre_cells[yy+y][xx+x]) ++neighbours;
			}
		}
	}

	return neighbours;
}

void updateCells(Game* game) {

	// Draw frame
	drawCells(game);

	// Update pre_cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			game->pre_cells[y][x] = game->cells[y][x];
		}
	}

	// Evaluate pre_cells into cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			int n = getPreNeighbours(game, y, x);
			//mvwprintw(game->window, y+1, x*2+1, "%d", n);
			//wrefresh(game->window);
			if (game->pre_cells[y][x]) {
				// Alive
				if (n < 2 || n > 3) game->cells[y][x] = 0;
			} else {
				// Dead
				if (n == 3) game->cells[y][x] = 1;
			}
		}
	}


	// Update again!
	++game->frame;
	usleep(UPDATE*1000);
	updateCells(game);
}

int main(int argc, char* argv[]) {
	initscr(); cbreak(); start_color();

	Game game;
	game.frame = 0;

	// Cells
	for (int y=0; y<HEIGHT; y++) {
		for (int x=0; x<WIDTH; x++) {
			game.cells[y][x] = 0;
			game.pre_cells[y][x] = 0;
		}
	}

	// Game Window
	game.window = newwin(HEIGHT+2, WIDTH*2+2, 0, 0);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	addRpentomino(&game, HEIGHT/2-2, WIDTH/2-2);
	//addGlider(&game, 0, 0);

	// Let's Play
	updateCells(&game);
	endwin();
	return 0;
}
