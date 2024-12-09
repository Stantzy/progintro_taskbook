#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#if !defined(A) && !defined(B) && !defined(C) && !defined(D)
#define A
#endif

int main(int argc, char **argv)
{
	int wr, status;
	int pid, fd, save_fd;

	if(argc < 3) {
		fprintf(
			stderr,
			"Specify the filename and the program to run\n"
		       );
		return 1;
	}

#if defined(A) || defined(C) || defined(D)
	save_fd = dup(1);
#elif defined(B)
	save_fd = dup(0);
#endif

	fflush(stdout);

#ifdef	A
	fd = open(argv[1], O_CREAT | O_WRONLY, 0666);
	if(fd == -1) {
		perror("open");
		return 2;
	}
	dup2(fd, 1);
	close(fd);
#elif	B
	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("open");
		return 2;
	}
	dup2(fd, 0);
	close(fd);
#elif	C
	fd = open(argv[1], O_CREAT | O_WRONLY | O_APPEND, 0666);
	if(fd == -1) {
		perror("open");
		return 2;
	}
	dup2(fd, 1);
	close(fd);
#elif	D
	fd = open(argv[1], O_WRONLY | O_APPEND);
	if(fd == -1) {
		perror("open");
		return 2;
	}
	dup2(fd, 1);
	close(fd);
#else
	fprintf(stderr, "Define A/B/C/D before compiling the program\n");
	return 3;
#endif

	pid = fork();
	if(pid == 0) {	/* child's process */
		execvp(argv[2], &(argv[2]));
		perror("fork");
		exit(1);	
	}

	/* parent's process */
#if defined(A) || defined(C) || defined(D)
	dup2(save_fd, 1);
#elif defined(B)
	dup2(save_fd, 0);
#endif
	close(save_fd);

	wr = wait(&status);
	if(wr != -1) {
		printf("Process with pid=%d finished.\n", wr);
	}

	return 0;
}
