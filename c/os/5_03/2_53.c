#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int fd_existing;
	int fd_newtxt;
	int fd_newbin;

	int c;
	int bytes_read;
	int current_line_len;
	int write_to_txt;

	if(argc != 4) {
		fprintf(stderr, "Enter the names of the three files\n");
		return 1;
	}

	fd_existing = open(argv[1], O_RDONLY);
	if(fd_existing == -1) {
		perror(argv[1]);
		return 2;
	}

	fd_newtxt = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd_newtxt == -1) {
		perror(argv[2]);
		close(fd_existing);
		return 3;
	}

	fd_newbin = open(argv[3], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd_newbin == -1) {
		perror(argv[3]);
		close(fd_existing);
		close(fd_newtxt);
		return 4;
	}

	current_line_len = 0;
	write_to_txt = 0;
	bytes_read = 0;
	while((bytes_read = read(fd_existing, &c, 1) > 0)) {
		if(c == '\n' || c == '\r') {
			write(fd_newbin, &current_line_len,
				sizeof(current_line_len));
			current_line_len = 0;

			if(write_to_txt) {
				write(fd_newtxt, &c, 1);
				write_to_txt = 0;
			}

			continue;
		}
		
		if(current_line_len == 0 && c == ' ')
			write_to_txt = 1;

		if(write_to_txt)
			write(fd_newtxt, &c, 1);

		current_line_len++;	
	}

	if(bytes_read == -1) {
		perror("bytes_read");
		close(fd_existing);
		close(fd_newtxt);
		close(fd_newbin);
		return 5;
	}

	close(fd_existing);
	close(fd_newtxt);
	close(fd_newbin);

	return 0;
}
