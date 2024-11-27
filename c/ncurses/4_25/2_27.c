#include <ncurses.h>
#include <unistd.h>

int move_star(int row, int col)
{
	enum { delay_duration = 10000 };
	int i, key = 0;

	timeout(0);

	for(i = 0; i < col; i++) {
		if((key = getch()) != ERR)
			return 1;
		move(row/2, i);
		addch('*');
		move(row/2, i-1);
		addch(' ');
		refresh();
		usleep(delay_duration);
	}
	
	for(i = col-1; i >= 0; i--) {
		if((key = getch()) != ERR)
			return 1;
		move(row/2, i);
		addch('*');
		move(row/2, i+1);
		addch(' ');
		refresh();
		usleep(delay_duration);
	}

	return 0;
}

int main()
{
	int row, col;
	int key_pressed = 0;

	initscr();
	getmaxyx(stdscr, row, col);
	curs_set(0);
	cbreak();
	noecho();

	while(!key_pressed) {
		key_pressed = move_star(row, col);
	}

	endwin();
	return 0;
}
