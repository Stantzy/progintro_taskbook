#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

enum { bufsize = 128 };

int main()
{
	int res, read_res;
	char buffer[bufsize];
	struct timeval tv = { 15, 0 };
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	printf("What is your name, please?\n");
	res = select(1, &readfds, NULL, NULL, &tv);
	if(res == -1) {
		if(errno == EINTR) {
			fprintf(stderr, "Interrupted by a signal\n");
			return EXIT_SUCCESS;
		} else {
			perror("select");
			return EXIT_FAILURE;
		}
	}
	else if(res == 0) {
		printf("Sorry, I'm terribly busy.\n");
	}
	else if(FD_ISSET(STDIN_FILENO, &readfds)) {
		read_res = read(STDIN_FILENO, buffer, bufsize);
		buffer[read_res - 1] = '\0';	/* removing '\n' at the end */
		printf("Nice to meet you, %s!\n", buffer);
	}

	return EXIT_SUCCESS;
}
