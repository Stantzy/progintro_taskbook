#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <errno.h>

struct timeval;

enum {
	timeout = 5,
	dgram_size = 508
};

enum {
	min_port_value = 1024,
	max_port_value = 49151
};

enum {
	ascii_printable_start = 32,
	ascii_printable_end = 126
};

void exit_with_error()
{
	fprintf(stderr, "Invalid ip address\n");
	exit(EXIT_FAILURE);
}

void check_port_is_valid(unsigned int port)
{
	if(port < min_port_value || port > max_port_value) {
		fprintf(
			stderr,
			"Specify the port value in range from 1024 to 49151\n"
		);
		exit(EXIT_FAILURE);
	}
}

unsigned short int get_port_as_num(const char *port)
{
	int i, num_len, mult = 1;
	unsigned short int number = 0;

	num_len = strlen(port);

	for(i = 1; i < num_len; i++) {
		mult *= 10;	/* next number position */
	}
	
	for(i = 0; i < num_len; i++) {
		number += (port[i] - '0') * mult;
		mult /= 10;	/* prev number position */
	}

	return number;
}

void init_sockaddr
(struct sockaddr_in *addr, unsigned short int h_port)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(h_port);
	(&(addr->sin_addr))->s_addr = htonl(INADDR_ANY); 
}

void check_result(int res, const char *func_name)
{
	if(res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
		return ;
	}

	if(res == -1) {
		perror(func_name);
		exit(EXIT_FAILURE);
	}
}

void print_dgram_with_info
(const char *dgram, int dlen, struct sockaddr_in *addr)
{
	char ip_str[INET_ADDRSTRLEN];
	int i;

	inet_ntop(AF_INET, &(addr->sin_addr), ip_str, INET_ADDRSTRLEN);
	printf(
		"Received a datagram from: %s:%d\n",
		ip_str,
		ntohs(addr->sin_port)
	);
	printf("Content:\n");
	for(i = 0; i < dlen; i++) {
		if(dgram[i] >= ascii_printable_start &&
			dgram[i] <= ascii_printable_end)
			write(STDOUT_FILENO, dgram + i, 1);
		else
			write(STDOUT_FILENO, "?", 1);
	}
	putchar('\n');
}

void set_timeout(struct timeval *tv, int sec, int usec, int sd)
{
	tv->tv_sec = sec;
	tv->tv_usec = usec;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	struct timeval tv;
	char buffer[dgram_size];
	int sockfd, ok, bytes_received;
	unsigned short int h_port = 0;
	socklen_t len;

	if(argc < 2) {
		fprintf(stderr, "Specify the port\n");
		return 1;
	}	

	h_port = get_port_as_num(argv[1]);
	check_port_is_valid(h_port);

	init_sockaddr(&addr, h_port);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	check_result(sockfd, "socket");

	set_timeout(&tv, timeout, 0, sockfd);

	ok = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	check_result(ok, "bind");

	for(;;) {
		memset(buffer, 0, dgram_size);

		len = sizeof(addr);
		bytes_received =
		recvfrom(
			sockfd,
			buffer,
			dgram_size,
			0,
			(struct sockaddr *)&addr,
			&len
		);
		check_result(bytes_received, "recsvfrom");

		if(bytes_received >= 0)
			print_dgram_with_info(buffer, bytes_received, &addr);
	}

	close(sockfd);
	return 0;
}

