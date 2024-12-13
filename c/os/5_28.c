#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#if !defined(A) && !defined(B) && !defined(C) && !defined(D)
#define A
#endif

enum {
	max_lines = 10,
	max_chars = 20
};

struct input_info {
	int count_lines;
	int flag_new_line;
	int flag_first_is_space;
	int count_chars;
	int flag_first_word;
	int print_newline_char;
};

int is_space_char(char c)
{
	return c == ' ' || c == '\t';
}

void update_info(struct input_info *inp, char c)
{
	(inp->count_chars)++;

	if(inp->flag_new_line) {
		inp->flag_first_word = 1;
		inp->flag_first_is_space = 0;
	}

	if(is_space_char(c))
		inp->flag_first_word = 0;

	if(is_space_char(c) && inp->flag_new_line)
		inp->flag_first_is_space = 1;

	if(c == '\n') {
		(inp->count_lines)++;
		inp->flag_new_line = 1;
		inp->count_chars = 0;
	} else {
		inp->flag_new_line = 0;
	}
}

int main(int argc, char **argv)
{
	int pid;
	int wr, status;
	int fd[2];
	FILE *pipe_read;
	char c;
	struct input_info inp = { 0, 1, 0, 0, 0, 0 };

	if(argc < 2) {
		fprintf(stderr, "Provide a prog as a CLI arg\n");
		return 1;
	}

	if(pipe(fd) == -1) {
		fprintf(stderr, "Pipe failed\n");
		return 2;
	}

	pid = fork();
	if(pid == -1) {
		perror("fork");
		return 3;
	}
	if(pid == 0) {
		dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);

		execvp(argv[1], argv + 1);
		perror(argv[1]);
		_exit(1);
	}

	close(fd[1]);
	pipe_read = fdopen(fd[0], "r");

	while((c = fgetc(pipe_read)) != EOF) {
		update_info(&inp, c);
#ifdef A
		if(inp.count_lines < max_lines)
			putchar(c);
#endif

#ifdef B
		if(inp.flag_first_is_space)
			putchar(c);
#endif

#ifdef C
		if(inp.count_chars < max_chars)
			putchar(c);
#endif

#ifdef D
		if(inp.flag_first_word) {
			putchar(c);
			inp.print_newline_char = 1;
		}

		if(c == '\n' && inp.print_newline_char) {
			putchar(c);
			inp.print_newline_char = 0;
		}
#endif
	}

#ifdef A
		putchar('\n');
#endif

	fclose(pipe_read);
	close(fd[0]);
	
	wr = wait(&status);
	if(wr == -1) {
		fprintf(stderr, "Can't run given program\n");
		return 4;
	}
	else if(WIFEXITED(status)) {
		printf("Exited with code: %d\n", WEXITSTATUS(status));
	}
	else {
		printf("Killed by signal: %d\n", WTERMSIG(status));
	}

	return 0;
}
