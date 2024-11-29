#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int wr, pid;

	pid = fork();
	if(pid == -1) {
		perror("fork");
		exit(1);
	}
	if(pid == 0) {
		printf("I'm child\n");
		exit(0);
	}

	while ((wr = wait4(-1, NULL, 0, NULL)) > 0) {
		if (wr > 0) {
			printf("Process %d completed\n", wr);
		}
	}

	exit(0);
}
