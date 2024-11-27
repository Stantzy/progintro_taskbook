#include <ncurses.h>
#include <stdlib.h>

enum { key_escape = 27, key_enter = 10 };
enum { std_color_pair = 1, selected_color_pair = 2 };
enum { fg = COLOR_WHITE, std_bg = COLOR_BLACK, selected_bg = COLOR_BLUE };

struct menu {
	char **items;
	int items_counter, scr_capacity;
	int f_idx;
	int s_idx;
};

static void init_ncurses(int *row, int *col)
{
	initscr();
	
	if(!has_colors()) {
		endwin();
		fprintf(stderr, "Can not show colors on a BW screen\n");
		exit(-2);
	}

	getmaxyx(stdscr, *row, *col);
	start_color();
	noecho();
	curs_set(0);
	keypad(stdscr, 1);
	cbreak();
	refresh();
}

static void init_color_pairs()
{
	init_pair(std_color_pair, fg, std_bg);
	init_pair(selected_color_pair, fg, selected_bg);
}

static void make_line(struct menu m, int out_idx, int y)
{
	move(y, 0);

	if(out_idx == m.s_idx)
		attrset(COLOR_PAIR(selected_color_pair));
	else
		attrset(COLOR_PAIR(std_color_pair));

	addstr(m.items[out_idx]);
}

static int center_screen(struct menu m)
{
	return m.scr_capacity / 2 - m.items_counter / 2;
}

static void make_screen(struct menu *m)
{
	int y, i;

	if(m->items_counter > m->scr_capacity)
		y = 0;
	else
		y = center_screen(*m);

	for(i = m->f_idx;
		i < m->scr_capacity + m->f_idx && i < m->items_counter;
		i++, y++
	) {
		make_line(*m, i, y);
	}	

	refresh();
}

static void switch_selected_line(struct menu *m, int d_y)
{
	m->s_idx += d_y;

	if(m->s_idx < 0)
		m->s_idx = m->items_counter - 1;
	else if(m->s_idx >= m->items_counter)
		m->s_idx = 0;

	if(m->s_idx < m->f_idx)
		m->f_idx = m->s_idx;
	else if(m->s_idx >= m->f_idx + m->scr_capacity)
		m->f_idx = m->s_idx - m->scr_capacity + 1;
}

static void handle_resize(struct menu *m, int *row, int *col)
{
	getmaxyx(stdscr, *row, *col);
	m->scr_capacity = *row;
}

int main(int argc, char **argv)
{
	int key;
	int row, col;
	struct menu m;

	if(argc < 3 || argc > 101) {
		fprintf(stderr, "Provide from 2 to 100 arguments\n");
		return -1;
	}

	init_ncurses(&row, &col);
	init_color_pairs();

	m.items = argv + 1;
	m.items_counter = argc - 1;
	m.scr_capacity = row;
	m.f_idx = 0;
	m.s_idx = 0;
	make_screen(&m);

	while((key = getch()) != key_escape) {
		switch(key) {
		case KEY_UP:
			switch_selected_line(&m, -1);
			break;
		case KEY_DOWN:
			switch_selected_line(&m, 1);
			break;
		case KEY_RESIZE:
			handle_resize(&m, &row, &col);
			break;
		case key_enter:
			endwin();
			return m.s_idx + 1;
		default:
			continue;
		}
		erase();
		make_screen(&m);
	}

	endwin();
	return 0;
}
