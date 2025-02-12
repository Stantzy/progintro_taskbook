#include <termios.h>
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

enum { del = 127 };

int is_separator(char c)
{
	return c == ' ' || c == '\t' || c == '\n';
}

int prefix_is_empty(char *prefix_start, char *prefix_end)
{
	int flag = 1;

	if(*prefix_start == '\0' && *prefix_end == '\0')
		return flag;
	
	for(; prefix_start < prefix_end; prefix_start++) {
		if(!is_separator(*prefix_start))
			flag = 0;
	}

	return flag;
}

char *get_prefix_start(char *prefix_cur, char *buf_start)
{
	while(!is_separator(*prefix_cur)) {
		if(prefix_cur == buf_start)
			return buf_start;
		prefix_cur--;
	}
	prefix_cur++;
	return prefix_cur;
}

int remove_char_from_buf(char **p_buf, char *buf_start)
{
	if(*p_buf - buf_start <= 0)
		return 0;

	(*p_buf)--;
	**p_buf = '\0';
	return 1;
}

void process_backspace(char **p_buf, char *buf_start)
{
	if(remove_char_from_buf(p_buf, buf_start)) {
		putchar('\b');
		putchar(' ');
		putchar('\b');
	}
}

void complete_word(char *buf_start, char *prefix_start, char **p_obuf_cur,
					FILE *dict, long offset)
{
	int len;
	
	fseek(dict, offset - 1, SEEK_SET);
	fgets(prefix_start, read_bufsize, dict);

	while(is_separator(*prefix_start))
		prefix_start++;

	len = strcspn(prefix_start, "\n");
	*p_obuf_cur = &(prefix_start[len]);
	prefix_start[len] = '\0';

	printf("\r%s", buf_start);
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

void output_multiple_matches(FILE *dict, long match_positions[match_bufsize], int count)
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
	pref_len = prefix_end - prefix_start;
	
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

int process_autocomplete(char **p_obuf_cur, char *buf_start, FILE *dict)
{
	int matches;
	long match_positions[match_bufsize];
	char *prefix_start;

	prefix_start = get_prefix_start(*p_obuf_cur, buf_start);

	matches = search_for_matches(buf_start, prefix_start, *p_obuf_cur,
								dict, match_positions);
	
	if(matches == 1) {
		complete_word(buf_start, prefix_start,
					p_obuf_cur, dict, match_positions[0]);
	} else if(matches != 0) {
		output_multiple_matches(dict, match_positions, matches);
		printf("%s", buf_start);
	}
	return 0;
}

int parse_input(FILE *out, FILE *dict)
{
	char read_buf[read_bufsize];
	char out_buf[out_bufsize];
	int read_res;
	char *p_rbuf, *p_obuf_cur;

	p_obuf_cur = out_buf;
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
				default:
					putchar(*p_rbuf);
					*p_obuf_cur = *p_rbuf;
					p_obuf_cur++;
					*p_obuf_cur = '\0';

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
