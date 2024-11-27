/* 4_36.c (2_32 remake) */

#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

enum { delay_duration = 100 };
enum { area_size = 5, color_count = 8 };
enum { key_escape = 27, key_space = 32 };


static const int all_colors[color_count] = {
	COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
	COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLOR_WHITE
};

struct area {
	int min_x, min_y;
	int max_x, max_y;
};

static void init_ncurses(int *row, int *col)
{
	initscr();
	
	if(!has_colors()) {
		endwin();
		fprintf(stderr, "Can not show colors on a BW screen\n");
		exit(1);
	}

	cbreak();
	timeout(delay_duration);
	keypad(stdscr, 1);
	noecho();
	curs_set(0);
	start_color();
	getmaxyx(stdscr, *row, *col);
}

static void make_area(const struct area *a)
{
	int x, y;
	
	init_pair(1, all_colors[0], all_colors[7]);
	attrset(COLOR_PAIR(1));

	for(y = a->min_y; y <= a->max_y; y++) {
		move(y, a->min_x);
		for(x = a->min_x; x <= a->max_x; x++)
			addch('*');
	}
}

static void change_colors(int fg, int bg)
{
	init_pair(1, all_colors[fg], all_colors[bg]);
	attrset(COLOR_PAIR(1));
	refresh();
}

static void adjust_value(int *n)
{
	if(*n < 0)
		*n = 7;
	else if(*n > 7)
		*n = 0;
}

int main()
{
	int row, col, key;
	int fg = 0, bg = 0;
	struct area a;

	init_ncurses(&row, &col);

	a.min_x = (col - area_size) / 2 - 1;
	a.min_y = (row - area_size) / 2 - 1;
	a.max_x = a.min_x + area_size - 1;
	a.max_y = a.min_y + area_size - 1;

	make_area(&a);
	refresh();
	
	while((key = getch()) != key_escape && key != key_space) {
		switch(key) {
		case KEY_LEFT:
			bg--;
			adjust_value(&bg);
			change_colors(fg, bg);	
			break;
		case KEY_RIGHT:
			bg++;
			adjust_value(&bg);
			change_colors(fg, bg);
			break;
		case KEY_UP:
			fg--;
			adjust_value(&fg);
			change_colors(fg, bg);
			break;
		case KEY_DOWN:
			fg++;
			adjust_value(&fg);
			change_colors(fg, bg);
			break;
		case KEY_RESIZE:

			break;
		}
	}

	endwin();
	return 0;
}
