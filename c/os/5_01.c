#define _LARGEFILE64_SOURCE

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd;
	int file_length = 0;

	if(argc != 2) {
		fprintf(stderr,
		"Please specify the file name as a command line argument\n");
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("Error opening file\n");
		return 2;
	}

	file_length = lseek64(fd, 0, SEEK_END);
	printf("%d\n", file_length);

	close(fd);

	return 0;
}
