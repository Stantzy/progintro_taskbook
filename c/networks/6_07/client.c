#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

struct timeval;

enum {
	timeout_usec = 0,
	timeout_sec = 15,
	dgram_size = 508
};

enum {
	min_port_value = 1024,
	max_port_value = 49151
};

int get_num(const char *str_int)
{
	int i, num_len, mult = 1;
	int number = 0;

	num_len = strlen(str_int);

	for(i = 1; i < num_len; i++) {
		mult *= 10;	/* next number position */
	}
	
	for(i = 0; i < num_len; i++) {
		number += (str_int[i] - '0') * mult;
		mult /= 10;	/* prev number position */
	}

	return number;
}

void check_port_is_valid(char *p)
{
	int port;
	port = get_num(p);
	if(port < min_port_value || port > max_port_value) {
		fprintf(
			stderr,
			"Specify the port value in range from 1024 to 49151\n"
		);
		exit(EXIT_FAILURE);
	}
}

int init_sockaddr
(struct sockaddr_in *addr, const char *h_ip, const char  *h_port)
{
	unsigned short int port;

	port = (unsigned short int)get_num(h_port);
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	if(inet_aton(h_ip, &(addr->sin_addr)) == 0) {
		fprintf(stderr, "Invalid IP address\n");
		exit(EXIT_FAILURE);
	}
	return 0;
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
	int sd, ok, dgram_len;
	char buffer[dgram_size];
	socklen_t len;

	if(argc < 3) {
		fprintf(
			stderr,
			"Specify the IP, port and length of the datagram\n"
        	);
		return 1;
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	check_port_is_valid(argv[2]);
	init_sockaddr(&addr, argv[1], argv[2]);
	dgram_len = atoi(argv[3]);
	len = sizeof(addr);
	set_timeout(&tv, timeout_sec, timeout_usec, sd);
	memset(buffer, 1, dgram_size);

	ok = sendto(sd, buffer, dgram_len, 0, (struct sockaddr *)&addr, len);
	if(ok == -1) {
		perror("sendto");
		return 1;
	}

	ok = recvfrom(sd, buffer, dgram_size, 0, (struct sockaddr *)&addr, &len);
	if(ok == -1 && errno == EAGAIN) {
		fprintf(stderr, "Timeout is over");
		return 1;
	}
	buffer[ok] = '\0';

	printf("%s\n", buffer);

	return 0;
}
