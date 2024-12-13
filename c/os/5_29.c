#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

enum {
	min_number = 1,
	max_number = 1000000
};

struct input_info {
	int chars;
	int lines;
};

int write_to_pipe(int fd)
{
	FILE *stream;
	int i, res;

	stream = fdopen(fd, "w");

	for(i = min_number; i <= max_number; i++) {
		res = fprintf(stream, "%d\n", i);
		if(res < 0)
			break;
	}
	
	fclose(stream);
	return res;
}

void analyze_input(int fd, struct input_info *inp)
{
	FILE *stream;
	char c;

	stream = fdopen(fd, "r");

	while((c = getc(stream)) != EOF) {
		(inp->chars)++;
		if(c == '\n')
			(inp->lines)++;
	}

	fclose(stream);
}

int main(int argc, char **argv)
{
	int p_1, p_2;
	int wr, status;
	int fd_1[2];
	int fd_2[2];
	int write_result;

	if(argc < 2) {
		fprintf(stderr, "Provide the name of program\n");
		return 1;
	}

	pipe(fd_1);
	pipe(fd_2);

	p_1 = fork();
	if(p_1 == -1) {
		perror("fork");
		return 2;
	}
	if(p_1 == 0) {
		dup2(fd_1[0], STDIN_FILENO);
		close(fd_1[0]);
		close(fd_1[1]);

		dup2(fd_2[1], STDOUT_FILENO);
		close(fd_2[0]);
		close(fd_2[1]);

		execvp(argv[1], argv + 1);
		perror(argv[1]);
		_exit(1);
	}

	p_2 = fork();
	if(p_2 == -1) {
		perror("fork");
		return 3;
	}
	if(p_2 == 0) {
		struct input_info inp = { 0, 0 };

		close(fd_1[0]);
		close(fd_1[1]);

		dup2(fd_2[0], STDIN_FILENO);
		close(fd_2[0]);
		close(fd_2[1]);

		analyze_input(STDIN_FILENO, &inp);
		printf("Chars = %d, lines = %d\n", inp.chars, inp.lines);

		_exit(0);
	}


	close(fd_1[0]);
	close(fd_2[0]);
	close(fd_2[1]);

	signal(SIGPIPE, SIG_IGN);
	write_result = write_to_pipe(fd_1[1]);
	if(write_result < 0)
		printf("Failure to transfer all numbers to the program\n");

	close(fd_1[1]);

	while((wr = wait(&status)) != -1) {
		printf("Process %d finished\n", wr);
		if(WIFEXITED(status)) {
			printf("Exit status: %d\n", WEXITSTATUS(status));
		} else {
			printf("Killed by sig: %d\n", WTERMSIG(status));
		}
	}

	return 0;
}

