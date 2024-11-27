#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

enum { base_area_size = 3 };
enum { delay_duration = 100 };
enum { key_escape = 27, key_space = 32 };


struct area {
	int h, w;
	int min_x, min_y;
	int max_x, max_y;
};

static void init_ncurses(int *row, int *col)
{
	clear();
	initscr();
	getmaxyx(stdscr, *row, *col);
	curs_set(0);
	noecho();
	cbreak();
	keypad(stdscr, 1);
	refresh();
}

static void make_area(const struct area *a)
{
	int i, j;

	for(i = a->min_y; i <= a->max_y; i++) {
		move(i, a->min_x);
		for(j = a->min_x; j <= a->max_x; j++)
			addch('*');
	}

	refresh();
}

static void change_boundaries(struct area *a, int row, int col)
{
	a->min_x = (col / 2) - (a->w / 2) - 1;
	a->min_y = (row / 2) - (a->h / 2) - 1;
	a->max_x = a->min_x + a->w - 1;
	a->max_y = a->min_y + a->h - 1;
}

static void resize_area(struct area *a, int row, int col)
{
	if(a->h < 1)
		a->h = 1;
	if(a->w < 1)
		a->w = 1;
	if(a->h >= row-2)
		a->h = row - 2;
	if(a->w >= col-2)
		a->w = col - 2;

	change_boundaries(a, row, col);
	clear();
	make_area(a);
}

int main()
{
	struct area a;
	int row, col, key;

	timeout(delay_duration);
	init_ncurses(&row, &col);

	a.h = base_area_size;
	a.w = base_area_size;
	a.min_x = (col / 2) - (a.w / 2) - 1;
	a.min_y = (row / 2) - (a.h / 2) - 1;
	a.max_x = a.min_x + a.w - 1;
	a.max_y = a.min_y + a.h - 1;

	make_area(&a);

	while((key = getch()) != key_escape && key != key_space) {
		switch(key) {
		case KEY_UP:
			a.h += 2;
			resize_area(&a, row, col);
			break;
		case KEY_DOWN:
			a.h -= 2;
			resize_area(&a, row, col);
			break;
		case KEY_LEFT:
			a.w -= 2;
			resize_area(&a, row, col);
			break;
		case KEY_RIGHT:
			a.w += 2;
			resize_area(&a, row, col);
			break;
		case KEY_RESIZE:
			break;
		}
	}

	endwin();

	return 0;
}
