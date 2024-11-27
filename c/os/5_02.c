#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_ARGC "You can provide no more than one file"
#define ERROR_READ "Reading error"
#define ERROR_OPEN_FILE "Error opening the file"
#define STRING_LEN(s) (sizeof(s) - 1)

enum { buf_size = 4096, block = 1024 };

void io_cycle(char *buf, int fd)
{
	int readed;
	while((readed = read(fd, buf, block)) != 0) {
		if(readed == -1) {
			write(2, ERROR_READ, STRING_LEN(ERROR_READ));
			exit(1);
		}
		write(1, buf, readed);
	}
}

void io_file(char *buf, char *filename)
{
	int fd;
	fd = open(filename, O_RDONLY);
	if(fd == -1) {
		write(2, ERROR_OPEN_FILE, STRING_LEN(ERROR_OPEN_FILE));
		exit(2);
	}
	io_cycle(buf, fd);
	close(fd);
}

int main(int argc, char **argv)
{
	char buf[buf_size];
	
	switch(argc) {
		case 1:
			io_cycle(buf, 0);
			break;	
		case 2:
			io_file(buf, argv[1]);
			break;
		default:
			write(2, ERROR_ARGC, STRING_LEN(ERROR_ARGC));
			return 1;
	}
	return 0;
}
