#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define FIFO_PERMS 0666
#define BUFSIZE 512

int create_fifo(const char *path, int perms)
{
	int ok;
	
	ok = mkfifo(path, perms);
	if(ok == -1 && errno != EEXIST)
		return -1;

	return 0;
}

void update_maxfd(int *cur_fd, int new_fd)
{
	if(new_fd > *cur_fd)
		*cur_fd = new_fd;
}

void end_program(int code, int fd_read, int fd_write)
{
	if(fd_read > 0)
		close(fd_read);
	if(fd_write > 0)
		close(fd_write);
	
	exit(code);
}

int main(int argc, char **argv)
{
	int ok, res;
	int fifo_read = 0, fifo_write = 0, max_fd = 0;
	char buffer[BUFSIZE];
	fd_set readfds;

	if(argc < 3) {
		fprintf(stderr, "Specify two FIFO files\n");
		end_program(EXIT_FAILURE, fifo_read, fifo_write);
	}
	
	ok = create_fifo(argv[1], FIFO_PERMS);
	if(ok == -1) {
		fprintf(stderr, "FIFO (read) creation error\n");
		end_program(EXIT_FAILURE, fifo_read, fifo_write);
	}

	ok = create_fifo(argv[2], FIFO_PERMS);
	if(ok == -1) {
		fprintf(stderr, "FIFO (write) creation error\n");
		end_program(EXIT_FAILURE, fifo_read, fifo_write);
	}

	fifo_read = open(argv[1], O_RDONLY | O_NONBLOCK);
	if(fifo_read == -1) {
		perror("Can't open FIFO (read)");
		end_program(EXIT_FAILURE, fifo_read, fifo_write);
	}
	update_maxfd(&max_fd, fifo_read);

	fifo_write = open(argv[2], O_WRONLY);
	if(fifo_write == -1 && errno != ENXIO) {
		perror("Can't open FIFO (write)");
		end_program(EXIT_FAILURE, fifo_read, fifo_write);
	}

	for(;;) {
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(fifo_read, &readfds);

		ok = select(max_fd + 1, &readfds, NULL, NULL, NULL);
		switch(ok) {
		case -1:
			if(errno != EINTR) {
				perror("select");
				end_program(EXIT_FAILURE, fifo_read, fifo_write);
			}
			if(errno == EINTR) {
				fprintf(stderr, "Interrupted by a signal\n");
			}
			break;
		case 0:
			break;
		default:
			if(FD_ISSET(STDIN_FILENO, &readfds)) {
				res = read(STDIN_FILENO, buffer, BUFSIZE);
				if(res == -1) {
					perror("read");
					end_program(EXIT_FAILURE, fifo_read, fifo_write);
				}
				if(res == 0) {
					end_program(EXIT_SUCCESS, fifo_read, fifo_write);
				}
				if(res > 0) {
					buffer[res] = '\0';
					write(fifo_write, buffer, res);
				}
			}

			if(FD_ISSET(fifo_read, &readfds)) {
				res = read(fifo_read, buffer, BUFSIZE);
				if(res == -1) {
					perror("read");
					end_program(EXIT_FAILURE, fifo_read, fifo_write);
				}
				if(res == 0) {
					printf("The interlocutor has left the chat\n");
					end_program(EXIT_SUCCESS, fifo_read, fifo_write);
				}
				if(res > 0) {
					buffer[res] = '\0';
					printf("%s", buffer);
				}
			}
			break;
		}
	}

	close(fifo_read);
	close(fifo_write);
	return EXIT_SUCCESS;
}
