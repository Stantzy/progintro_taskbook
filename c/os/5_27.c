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

int main(int argc, char **argv)
{
	int p1, wr, status;
	int fd[2];
	int write_result;

	if(argc < 2) {
		fprintf(stderr, "Provide the name of program\n");
		return 1;
	}

	pipe(fd);

	p1 = fork();
	if(p1 == -1) {
		perror("fork");
		return 2;
	}
	if(p1 == 0) {
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		execvp(argv[1], argv + 1);
		perror(argv[1]);
		exit(1);
	}

	close(fd[0]);

	signal(SIGPIPE, SIG_IGN);
	write_result = write_to_pipe(fd[1]);
	if(write_result < 0)
		printf("Failure to transfer all numbers to the program\n");

	close(fd[1]);

	wr = wait(&status);
    if(wr == -1) {
		fprintf(stderr, "Failure to start the given program\n");
		return 3;
	} else if(WIFEXITED(status)) {
		printf("Exited with status: %d\n", WEXITSTATUS(status));
	} else {
		printf("Killed with code: %d\n", WTERMSIG(status));
	}

	return 0;
}
