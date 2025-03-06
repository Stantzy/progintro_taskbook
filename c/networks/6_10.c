#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t str_cnt = 0, chr_cnt = 0, sigint_flag = 0;

enum {
	timeout_sec = 5,
	timeout_usec = 0,
	max_num_pos = 11,
	bufsize = 256
};

void reverse(char *numstr, int numlen)
{
	int i, j;
	char c;

	for(i = 0, j = numlen - 1; i < j; i++, j--) {
		c = numstr[i];
		numstr[i] = numstr[j];
		numstr[j] = c;
	}
}

void get_num_as_str(int num, char *numstr, int *numlen)
{
	int i = 0;

	*numlen = 0;
	do {
		numstr[i] = num % 10 + '0';
		(*numlen)++;
		i++;
		num /= 10;
	} while(num > 0);
	numstr[i] = '\0';
	reverse(numstr, *numlen);
}

void sigint_handler(int s)
{
	int numlen = 0, save_errno = errno;
	char numstr[max_num_pos];
	char start_str[] = "str_cnt = ";
	char end_str[] = " chr_cnt = ";
	char linefeed[] = "\n";

	if(sigint_flag != 0)
		exit(1);

	signal(SIGINT, sigint_handler);
	sigint_flag = 1;

	write(STDOUT_FILENO, start_str, sizeof(start_str) - 1);
	get_num_as_str((int)str_cnt, numstr, &numlen);
	write(STDOUT_FILENO, numstr, numlen);

	write(STDOUT_FILENO, end_str, sizeof(end_str) - 1);
	get_num_as_str((int)chr_cnt, numstr, &numlen);
	write(STDOUT_FILENO, numstr, numlen);

	write(STDOUT_FILENO, linefeed, sizeof(linefeed) - 1);

	errno = save_errno;
}

int count_chars(const char *str)
{
	int counter = 0;

	for(; *str != '\0'; str++)
		counter++;

	return counter;
}

int main()
{
	struct timeval tv;
	int res, rd_res;
	char buffer[bufsize];
	fd_set readfds;
	
	signal(SIGINT, sigint_handler);

	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds);

	get_num_as_str(156, buffer, &res);

	for(;;) {
		tv.tv_sec = timeout_sec;
		tv.tv_usec = timeout_usec;
		res = select(1, &readfds, NULL, NULL, &tv);
		switch(res) {
		case -1:
			if(errno != EINTR) {
				perror("select");
				return 1;
			}
			break;
		case 0:
			sigint_flag = 0;
			printf("Are you here?\n");
			break;
		default:
			if(FD_ISSET(STDIN_FILENO, &readfds)) {
				sigint_flag = 0;
				rd_res = read(STDIN_FILENO, buffer, bufsize);
				buffer[rd_res] = '\0';
				if(rd_res == -1) {
					perror("read");
					return 1;
				}
				str_cnt++;
				chr_cnt += count_chars(buffer);
			}
			break;
		}
	}

	return 0;
}
