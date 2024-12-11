#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

enum { size = 1024 };
const char test_msg[] = "The bear is white.\nThe bird is blue.\n";

int main()
{
	int fd[2];	/* fd[0] for reading, fd[1] for writing */
	int pid, wr, status;
	int bytes;
	char buffer[size];

	pipe(fd);
	printf("fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);

	pid = fork();
	if(pid == -1) {
		perror("pid");
		return 1;
	}
	if(pid == 0) { /* child process */
		close(fd[0]);
		write(fd[1], test_msg, sizeof(test_msg) - 1);
		close(fd[1]);
		exit(0);
	}
	/* parent process */
	close(fd[1]);
	
	while((bytes = read(fd[0], buffer, sizeof(buffer))) > 0) {
		write(1, buffer, bytes);
	}
	
	close(fd[0]);
	wr = wait(&status);

	return 0;
}
