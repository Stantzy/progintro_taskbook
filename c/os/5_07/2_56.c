#include "item.h"
#include "strings.h"

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdlib.h>

#define ARGC_ERR "Specify 2 files to join and " \
                "the name of the file to be created\n"
#define ARGC_ERR_LEN (sizeof(ARGC_ERR) - 1)

int main(int argc, char **argv)
{
	int in_fd1, in_fd2, out_fd;
	int status = 0;

	if(argc != 4) {
		write(STDERR_FILENO, ARGC_ERR, ARGC_ERR_LEN);
		status = 1;
		return status;
	}

	in_fd1 = open(argv[1], O_RDONLY);
	if(in_fd1 == -1) {
		perror("Error opening file");
        status = 2;
		goto error;
	}

	in_fd2 = open(argv[2], O_RDONLY);
	if(in_fd2 == -1) {
		perror("Error opening file");
        status = 2;
		goto error;
	}

	out_fd = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, 0666);
	if(out_fd == -1) {
		perror("Error opening file");
        status = 2;
		goto error;
	}

    join_tables(in_fd1, in_fd2, out_fd);

	close(in_fd1);
	close(in_fd2);
	close(out_fd);
	return status;

	error:
		if(in_fd1 != -1)
			close(in_fd1);
		if(in_fd2 != -1)
			close(in_fd2);
		if(out_fd != -1)
			close(out_fd);
		return status;
}