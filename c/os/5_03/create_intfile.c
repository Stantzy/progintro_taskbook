#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd_new;
	int num;

	if(argc != 2) {
		perror("Specify the name of the file to be created\n");
		return 1;
	}

	fd_new = open(argv[1], O_CREAT | O_WRONLY, 0666);
	if(fd_new == -1) {
		perror(argv[1]);
		return 2;
	}

	while(scanf("%d", &num) == 1)
		write(fd_new, &num, sizeof(num));

	close(fd_new);
	return 0;
}
