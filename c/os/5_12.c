#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int pid;
	int status, wr;

	if(argc < 2) {
		fprintf(stderr, 
			"Usage: %s command [arguments...]\n",
			argv[0]);
		return 1;
	}
	
	pid = fork();
	if(pid == -1) {
		perror("fork");
		return 2;
	}
	if(pid == 0) {
		execvp(argv[1], argv + 1);
		perror(argv[1]);
		exit(1);
	} 

	wr = wait(&status);
	if(wr == -1) {
		printf("Program %s launch error\n", argv[1]);
	} else {
		if(WIFEXITED(status)) {
			printf(
				"%s exited with code=%d\n",
				argv[1],
				WEXITSTATUS(status)
				);
		} else {
			printf(
				"%s killed by signal=%d\n",
				argv[1],
				WTERMSIG(status)
				);
		}
	}
	
	return 0;
}
