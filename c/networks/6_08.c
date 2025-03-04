#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

struct timeval;

enum {
	qlen = 5,
	bufsize = 512,
	min_port_value = 1024,
	max_port_value = 49151
};

enum {
	timeout_sec = 10,
	timeout_usec = 0
};

void daemonize_self()
{
	int pid;

	close(1);
	close(2);
	close(3);

	open("/dev/null", O_RDONLY);
	open("/dev/null", O_WRONLY);
	open("/dev/null", O_WRONLY);

	pid = fork();
	if(pid > 0)
		exit(EXIT_SUCCESS);
	setsid();

	pid = fork();
	if(pid > 0)
		exit(EXIT_SUCCESS);
}

int check_port_is_valid(unsigned short port)
{
	return (port >= min_port_value) && (port <= max_port_value);
}

void init_sockaddr(struct sockaddr_in *addr, unsigned short port)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	(&(addr->sin_addr))->s_addr = INADDR_ANY;
}

void check_res(int res, int sd, char *fname)
{
	if(res == -1) {
		perror(fname);
		close(sd);
		exit(EXIT_FAILURE);
	}
}

void set_timeout(struct timeval *tv, int sec, int usec, int sd)
{
	tv->tv_sec = sec;
	tv->tv_usec = usec;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, tv, sizeof(*tv));
}

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	struct timeval tv;
	int opt = 1;
	int ls, ok, buf_len;
	unsigned short port;
	char buffer[bufsize];
	time_t cur_time;
	struct tm now;

	if(argc < 2) {
		fprintf(stderr, "Specify the port\n");
		return EXIT_FAILURE;
	}

	port = (unsigned short)atoi(argv[1]);
	if(check_port_is_valid(port) == 0) {
		fprintf(
			stderr,
			"Specify the port value in range from 1024 to 49151\n"
		);
		return EXIT_FAILURE;
	}

	ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	check_res(ls, ls, "socket");
	ok = setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	check_res(ok, ls, "setsockopt");
    
	init_sockaddr(&addr, port);
	set_timeout(&tv, timeout_sec, timeout_usec, ls);
    
	ok = bind(ls, (struct sockaddr *)&addr, sizeof(addr));
	check_res(ok, ls, "bind");

	ok = listen(ls, qlen);
	check_res(ok, ls, "listen");

	/* daemonize_self(); */

	for(;;) {
		int cs;
		char ip[INET_ADDRSTRLEN];
		socklen_t slen;
		
		slen = sizeof(addr);
		cs = accept(ls, (struct sockaddr *)&addr, &slen);
		if(cs == -1 && errno == EAGAIN) {
			continue;
		} else if(cs == -1) {
			close(cs);
			continue;
		}
		inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);

		cur_time = time(NULL);
		now = *localtime(&cur_time);
		buf_len = snprintf(
			buffer,
			bufsize,
			"%d-%02d-%02d %02d:%02d:%02d %s:%d\n",
			now.tm_year + 1900,
			now.tm_mon + 1,
			now.tm_mday,
			now.tm_hour,
			now.tm_min,
			now.tm_sec,
			ip,
			addr.sin_port
			);

		write(cs, buffer, buf_len);
		close(cs);
	}

	close(ls);
	return EXIT_SUCCESS;
}
