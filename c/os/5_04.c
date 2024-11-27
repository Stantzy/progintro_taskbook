#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

enum { buf_size = 4096 };

void count_lines(char *buffer, int length, int *counter)
{
	int i;
	for(i = 0; i < length; i++) {
		if(buffer[i] == '\n')
			(*counter)++;
	}
}

int main(int argc, char **argv)
{
	int fd;
	char buffer[buf_size];
	int bytes_read;
	int counter = 0;

	if(argc != 2) {
		perror("Specify the name of only one file\n");
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if(fd == -1) {
		perror("Can't open the file\n");
		return 2;
	}

	while((bytes_read = read(fd, buffer, buf_size)) != 0) {
		if(bytes_read == -1) {
			close(fd);
			perror("Can't read the file\n");
			return 3;
		}
		count_lines(buffer, bytes_read, &counter);
	}

	printf("Line counter = %d\n", counter);
	close(fd);
	return 0;	
}
