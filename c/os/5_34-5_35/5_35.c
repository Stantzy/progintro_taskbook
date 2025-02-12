#include <termios.h>
#include <bits/termios-c_cc.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

enum {
	expected_argc = 3,
	match_bufsize = 256,
	read_bufsize = 1024,
	out_bufsize = 4096
};

enum {
	erase_word = 23,
	escape = 27,
	right_arrow = 67,
	left_arrow = 68,
	del = 127
};

void fill_null(char *arr, int size)
{
	int i;
	for(i = 0; i < size; i++)
		arr[i] = 0;
}

int is_separator(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

int prefix_is_empty(char *prefix_start, char *prefix_end)
{
	int flag = 1;

	if(*prefix_start == '\0' && *prefix_end == '\0')
		return flag;
	if(prefix_start == prefix_end) {
		flag = 0;
		return flag;
	}

	for(; prefix_start < prefix_end; prefix_start++) {
		if(!is_separator(*prefix_start))
			flag = 0;
	}

	return flag;
}

char *get_prefix_start(char *prefix_cur, char *buf_start)
{
	if(prefix_cur > buf_start && !is_separator(*(prefix_cur - 1)))
		prefix_cur--;
	
	while(!is_separator(*prefix_cur)) {
		if(prefix_cur == buf_start)
			return buf_start;
		prefix_cur--;
	}
	prefix_cur++;
	return prefix_cur;
}

char *get_prefix_end(char *prefix_cur, char *buf_start)
{
	while(*prefix_cur != '\0' && !is_separator(*prefix_cur)) {
		prefix_cur++;
	}
	prefix_cur--;
	return prefix_cur;
}

int remove_char_from_buf(char **pp_buf, char *buf_start)
{
	char *tmp;

	if(*pp_buf - buf_start <= 0)
		return 0;

	(*pp_buf)--;
	if(*(*pp_buf + 1) == '\0') {
		**pp_buf = '\0';
		return 1;
	} else {
		tmp = *pp_buf;
		while(*(tmp + 1) != '\0') {
			*tmp = *(tmp + 1);
			tmp++;
		}
		*tmp = '\0';
		return 2;
	}

	return 1;
}

void process_backspace(char **pp_buf, char *buf_start)
{
	int result, count_to_back = 0;
	char *tmp;
	result = remove_char_from_buf(pp_buf, buf_start);
	if(result == 1) {
		putchar('\b');
		putchar(' ');
		putchar('\b');
	} else if(result == 2) {
		putchar('\b');
		tmp = *pp_buf;
		while(*tmp != '\0') {
			putchar(*tmp);
			count_to_back++;
			tmp++;
		}
		putchar(' ');
		putchar('\b');
		while(count_to_back > 0) {
			putchar('\b');
			count_to_back--;
		}
	}
}

void complete_word(char *buf_start, char *prefix_start, char *prefix_end,
			char **pp_obuf_cur, FILE *dict, long offset)
{
	int len;
	char *tmp, *p_tail;
	char tail[out_bufsize];
	
	strcpy(tail, prefix_end + 1);

	fseek(dict, offset - 1, SEEK_SET);
	fgets(prefix_start, out_bufsize, dict);

	while(is_separator(*prefix_start))
		prefix_start++;

	len = strcspn(prefix_start, "\n");
	*pp_obuf_cur = &(prefix_start[len]);
	tmp = *pp_obuf_cur;
	p_tail = tail;
	while(*p_tail != '\0') {
		*tmp = *p_tail;
		tmp++;
		p_tail++;
	}
	*tmp = '\0';

	printf("\r%s", buf_start);
	tmp = *pp_obuf_cur;
	while(*tmp != '\0') {
		putchar('\b');
		tmp++;
	}
}

void print_match(char *match)
{
	int len;

	len = strlen(match);
	if(len > 0 && match[len - 1] == '\n')
		len--;
	fwrite(match, sizeof(char), len, stdout);
	putc(' ', stdout);
}

void output_multiple_matches(FILE *dict,
			long match_positions[match_bufsize], int count)
{
	int i;
	char word[out_bufsize];

	printf("\n");
	if(count == -1) {	/* prefix is empty */
		fseek(dict, 0, SEEK_SET);
		while((fgets(word, out_bufsize, dict)) != NULL) {
			print_match(word);
		}
	} else {
		for(i = 0; i < count; i++) {
			fseek(dict, match_positions[i] - 1, SEEK_SET);
			fgets(word, out_bufsize - 1, dict);
			print_match(word);
		}
	}
	printf("\n");
}

int search_for_matches(char *buf_start, char *prefix_start, char *prefix_end,
									FILE *dict, long match_pos[match_bufsize])
{
	int count_matches = 0, match_flag = 1, new_line = 1;
	char c;
	long save, pos = 0;
	int pref_len = 0, i = 0;

	if(prefix_is_empty(prefix_start, prefix_end))
		return -1;

	while(is_separator(*prefix_start))
		prefix_start++;
	if(prefix_start == prefix_end)
		pref_len = 1;
	else
		pref_len = prefix_end - prefix_start + 1;
	
	fseek(dict, 0, SEEK_SET);
	while((c = fgetc(dict)) != EOF) {
		if(new_line) {
			save = ftell(dict);
			new_line = 0;
		}

		if(match_flag && c == *(prefix_start + i)) {
			if(pref_len - i - 1 == 0) {
				match_pos[pos] = save;
				count_matches++;
				pos++;
				i = 0;
			} else {
				i++;
			}
		} else {
			match_flag = 0;
			i = 0;
		}

		if(c != '\n') {
			continue;
		} else if(c == '\n') {
			new_line = 1;
			match_flag = 1;
		}
	}

	return count_matches;
}

int process_autocomplete(char **pp_obuf_cur, char *buf_start, FILE *dict)
{
	int matches;
	long match_positions[match_bufsize];
	char *prefix_start, *prefix_end;
	int buf_len, count_b;

	prefix_start = get_prefix_start(*pp_obuf_cur, buf_start);
	if(*pp_obuf_cur > buf_start)
		prefix_end = *pp_obuf_cur - 1;
	else if(*pp_obuf_cur == buf_start)
		prefix_end = prefix_start;

	matches = search_for_matches(buf_start, prefix_start, prefix_end,
								dict, match_positions);
	
	if(matches == 1) {
		complete_word(buf_start, prefix_start, prefix_end,
					pp_obuf_cur, dict, match_positions[0]);
	} else if(matches != 0) {
		output_multiple_matches(dict, match_positions, matches);
		buf_len = printf("%s", buf_start);
		count_b = buf_len - (int)(*pp_obuf_cur - buf_start);
		for(; count_b > 0; count_b--)
			putchar('\b');
	}
	return 0;
}

int handle_pressed_arrow(char arrow, char *out_buf_start,
										char **pp_obuf_cur)
{
	if(arrow == left_arrow && *pp_obuf_cur == out_buf_start)
		return 1;
	else if(arrow == right_arrow && **pp_obuf_cur == '\0')
		return 1;
	
	if(arrow == left_arrow) {
		putchar('\b');
		*pp_obuf_cur = *pp_obuf_cur - 1;
	} else if(arrow == right_arrow) {
		putchar(**pp_obuf_cur);
		*pp_obuf_cur = *pp_obuf_cur + 1;
	}

	return 0;
}

int process_escape_seq(char *read_buf, int *read_res,
				char *out_buf_start, char **pp_obuf_cur, int *flag)
{
    enum { esc_type_pos = 2, seq_max_len = 7 };
	char escape_seq[seq_max_len];
    int i;
    
	fill_null(escape_seq, seq_max_len);

    for(i = 0; i < *read_res; i++) {
		if(i > 0 && read_buf[i] == escape)
			break;
        escape_seq[i] = read_buf[i];
    }
	
	switch(escape_seq[esc_type_pos]) {
		case left_arrow:
		case right_arrow:
			if(handle_pressed_arrow(escape_seq[esc_type_pos], 
								out_buf_start, pp_obuf_cur)) {
				*flag = 1;
			}
			break;
		default:
			break;
	}

    *read_res -= strlen(escape_seq);	/* ? */
	
	return 0;
}

char *get_str_end(char *str)
{
	while(*str != '\0')
		str++;

	return str;
}

void update_out_buf(char *out_buf, char **p_obuf_cur, char c)
{
	char *out_buf_end;

	out_buf_end = get_str_end(out_buf);
	if((out_buf_end - out_buf) < (out_bufsize - 1))
		*(out_buf_end + 1) = '\0';

	while(out_buf_end > *p_obuf_cur) {
		*out_buf_end = *(out_buf_end - 1);
		out_buf_end--;
	}
	**p_obuf_cur = c;
	*p_obuf_cur = *p_obuf_cur + 1;
}

void print_updated_buf(char *out_buf, char *p_obuf_cur, int offset_back)
{
	char *buf_end;
	int tmp = offset_back;

	printf("\r%s", out_buf);
	tmp = offset_back;
	for(tmp = offset_back; tmp > 0; tmp--)
		putchar(' ');
	for(tmp = offset_back; tmp > 0; tmp--)
		putchar('\b');
		
	buf_end = get_str_end(out_buf);
	printf("\r%s", out_buf);
	while(buf_end != p_obuf_cur) {
		putchar('\b');
		buf_end--;
	}
}

int erase_prev_word(char *start_buf, char **p_obuf_cur)
{
	char *tmp, *tail;
	int offset;

	tmp = *p_obuf_cur;
	tail = *p_obuf_cur;

	if(*p_obuf_cur == start_buf)
		return 1;

	if(is_separator(**p_obuf_cur) || **p_obuf_cur == '\0')
		while(*tmp == '\0' || (is_separator(*tmp) && tmp > start_buf))
			tmp--;
	
	while(!is_separator(*tmp) && tmp > start_buf)
		tmp--;

	offset = tail - tmp;
	*p_obuf_cur = tmp;

	while(*tail != '\0') {
		*tmp = *tail;
		tmp++;
		tail++;
	}
	*tmp = '\0';

	print_updated_buf(start_buf, *p_obuf_cur, offset);

	return 0;
}

int parse_input(FILE *out, FILE *dict)
{
	char read_buf[read_bufsize];
	char out_buf[out_bufsize];
	int read_res, arrow_pressed = 0;
	char *p_rbuf, *p_obuf_cur;

	p_obuf_cur = out_buf;
	*p_obuf_cur = '\0';
	while((read_res = read(0, read_buf, sizeof(read_buf))) > 0) {
			for(p_rbuf = read_buf; (p_rbuf - read_buf) < read_res; p_rbuf++) {
			switch(*p_rbuf) {
				case '\4':
					if(p_obuf_cur == out_buf)
						goto ret;
					break;
				case '\t':
					process_autocomplete(&p_obuf_cur, out_buf, dict);
					break;
				case '\n':
					fprintf(out, "%s\n", out_buf);
					putchar(*p_rbuf);
					p_obuf_cur = out_buf;
					*p_obuf_cur = '\0';
					break;
				case del:
				case '\b':
					process_backspace(&p_obuf_cur, out_buf);
					break;
    			case escape:
					process_escape_seq(read_buf, &read_res,
						out_buf, &p_obuf_cur, &arrow_pressed);
					break;
				case erase_word:
					erase_prev_word(out_buf, &p_obuf_cur);
					break;
				default:
					if(*p_obuf_cur != '\0') {
						update_out_buf(out_buf, &p_obuf_cur, *p_rbuf);
						print_updated_buf(out_buf, p_obuf_cur, 0);
					} else {
						putchar(*p_rbuf);
						*p_obuf_cur = *p_rbuf;
						p_obuf_cur++;
						if(!arrow_pressed) 
							*p_obuf_cur = '\0';
						arrow_pressed = 0;
					}
					break;
			}
			fflush(stdout);
		}
	}

ret:
	return 0;
}

void set_noncannonical_mode(struct termios *ts1)
{
	ts1->c_lflag &= ~(ICANON | ECHO);
	ts1->c_lflag &= ISIG;
	ts1->c_cc[VMIN] = 1;
	ts1->c_cc[VTIME] = 0;
	ts1->c_cc[VEOF] = 0;

	if(tcsetattr(0, TCSANOW, ts1) != 0) {
		perror("tcsetattr");
		exit(1);
	}
}
int main(int argc, char **argv)
{
	FILE *out_fd, *dict_fd;
	struct termios ts1, ts2;

	if(!isatty(0)) {
		fprintf(stderr, "Not a terminal\n");
		return 1;
	}

	if(argc != expected_argc) {
		fprintf(stderr, "Specify two file names as a CL args\n");
		return 2;
	}

	out_fd = fopen(argv[1], "w");
	if(!out_fd) {
		fprintf(stderr, "Can't open the file\n");
		return 3;
	}

	dict_fd = fopen(argv[2], "r");
	if(!dict_fd) {
		fprintf(stderr, "Can't open the file\n");
		fclose(out_fd);
		return 4;
	}

	if(tcgetattr(0, &ts1) != 0) {
		perror("tcgetattr");
		exit(1);
	}
	memcpy(&ts2, &ts1, sizeof(ts1));
	set_noncannonical_mode(&ts1);
	parse_input(out_fd, dict_fd);
	if(tcsetattr(0, TCSANOW, &ts2) != 0) {	/* restore settings */
		perror("tcsetattr");
		return 5;
	}
	fclose(out_fd);
	fclose(dict_fd);

	return 0;
}
