#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

enum { area_size = 10, min_terminal_size = 12 };
enum { key_space = 32, key_escape = 27 };
enum { delay_duration = 100,
	min_delay = 100,
	max_delay = 3000,
	delay_step = 100
};

struct area {
	int min_x, min_y, max_x, max_y;
};

struct hash_char {
	int cur_x, cur_y, dx, dy;
	int direction;	/* 1 or -1 */
};

static void show_hash_char(struct hash_char *hc)
{
	move(hc->cur_y, hc->cur_x);
	addch('#');
}

static void check_terminal(int h, int w)
{
	if(h < min_terminal_size || w < min_terminal_size) {
		endwin();
		fprintf(stderr, "The terminal is too small for output\n");
		exit(1);
	}
}

static void init_ncurses(int *row, int *col)
{	
	initscr();
	getmaxyx(stdscr, *row, *col);
	curs_set(0);
	cbreak();
	keypad(stdscr, 1);
	noecho();
}

static void show_area(const struct area *a)
{
	int x, y;	
	
	for(y = a->min_y; y <= a->max_y; y++) {
		move(y, a->min_x);
		for(x = a->min_x; x <= a->max_x; x++)
			addch('*');
	}
	
	refresh();
}

static void set_direction(struct hash_char *hc, int dx, int dy)
{
	hc->dx = dx;
	hc->dy = dy;
}

static void hide_char(struct hash_char *hc)
{
	move(hc->cur_y, hc->cur_x);
	addch('*');
}

static void move_char(struct hash_char *hc, struct area *a)
{
	if(hc->cur_x == a->max_x && hc->cur_y == a->min_y) {
		if(hc->direction == 1)
			set_direction(hc, 0, 1);
		else
		if(hc->direction == -1)
			set_direction(hc, -1, 0);
	}
	else
	if(hc->cur_x == a->max_x && hc->cur_y == a->max_y) {
		if(hc->direction == 1)
			set_direction(hc, -1, 0);
		else
		if(hc->direction == -1)
			set_direction(hc, 0, -1);
	}
	else
	if(hc->cur_x == a->min_x && hc->cur_y == a->max_y) {
		if(hc->direction == 1)
			set_direction(hc, 0, -1);
		else
		if(hc->direction == -1)
			set_direction(hc, 1, 0);
	}
	else
	if(hc->cur_x == a->min_x && hc->cur_y == a->min_y) {
		if(hc->direction == 1)
			set_direction(hc, 1, 0);
		else
		if(hc->direction == -1)
			set_direction(hc, 0, 1);
	}
	
	hide_char(hc);
	hc->cur_x += hc->dx;
	hc->cur_y += hc->dy;
	show_hash_char(hc);
}

static void change_delay(int *delay, int key)
{
	if(key == KEY_LEFT)
		*delay -= delay_step;
	if(*delay < min_delay)
		*delay = min_delay;

	if(key == KEY_RIGHT)
		*delay += delay_step;
	if(*delay > max_delay)
		*delay = max_delay;

	timeout(*delay);
}

static void handle_resize(struct hash_char *hc, struct area *a,
				int *col, int *row)
{
	clear();
	getmaxyx(stdscr, *row, *col);
	check_terminal(*row, *col);
	
	a->min_x = (*col-area_size) / 2 - 1;
	a->min_y = (*row-area_size) / 2 - 1;
	a->max_x = a->min_x + area_size - 1;
	a->max_y = a->min_y + area_size - 1;

	hc->cur_x = a->min_x;
	hc->cur_y = a->min_y;

	show_area(a);
	show_hash_char(hc);
}

int main()
{
	int row, col, key;
	int delay;
	struct hash_char hc;
	struct area a;


	init_ncurses(&row, &col);
	check_terminal(row, col);
	timeout(delay_duration);

	delay = delay_duration;

	a.min_x = (col-area_size) / 2 - 1;
	a.min_y = (row-area_size) / 2 - 1;
	a.max_x = a.min_x + area_size - 1;
	a.max_y = a.min_y + area_size - 1;

	hc.cur_x = a.min_x;
	hc.cur_y = a.min_y;
	hc.dx = 0;
	hc.dy = 1;
	hc.direction = 1;

	show_area(&a);
	show_hash_char(&hc);

	while((key = getch()) != key_escape) {
		switch(key) {
		case key_space:
			hc.dx *= -1;
			hc.dy *= -1;
			hc.direction *= -1;
			break;
		case KEY_LEFT:
			change_delay(&delay, key);
			break;
		case KEY_RIGHT:
			change_delay(&delay, key);
			break;
		case ERR:
			move_char(&hc, &a);
			break;
		case KEY_RESIZE:
			handle_resize(&hc, &a, &col, &row);
		}		
	}

	endwin();

	return 0;
}
