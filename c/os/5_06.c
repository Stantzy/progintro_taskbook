#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

enum { buf_size = 4096 };

void encrypt_data(char *buf, int len, int key)
{
	int i;

	for(i = 0; i < len; i += sizeof(key)) {
		*((int*)(buf + i)) ^=  key;
	}
}

int main(int argc, char **argv)
{
	int fd, key, bytes_readed;
	char buffer[buf_size];

	if(argc != 3) {
		perror("Specify the filename and the key");
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror("Can't open the file");
		return 2;
	}

	sscanf(argv[2], "%d", &key);
	while((bytes_readed = read(fd, buffer, buf_size)) != 0) {
		if(bytes_readed == -1) {
			perror("Error reading file");
			close(fd);
			return 3;
		}
		encrypt_data(buffer, bytes_readed, key);
		lseek(fd, -bytes_readed, SEEK_CUR);
		write(fd, buffer, bytes_readed);
	}

	close(fd);
	return 0;
}
