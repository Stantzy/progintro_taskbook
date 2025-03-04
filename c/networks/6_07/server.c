#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

struct timeval;

enum {
	timeout_usec = 0,
	timeout_sec = 5,
	dgram_size = 508
};

enum {
	min_port_value = 1024,
	max_port_value = 49151
};

void daemonize_self()
{
	int pid;

	close(0);
	close(1);
	close(2);
	open("/dev/null", O_RDONLY);	/* stdout */
	open("/dev/null", O_WRONLY);	/* stdin */
	open("/dev/null", O_WRONLY);	/* stderr */
	chdir("/");

	pid = fork();
	if(pid > 0)
		exit(EXIT_SUCCESS);
	
	setsid();
	pid = fork();
	if(pid > 0)
		exit(EXIT_SUCCESS);
}

void set_timeout(struct timeval *tv, int sec, int usec, int sd)
{
	tv->tv_sec = sec;
	tv->tv_usec = usec;
	setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, tv, sizeof(*tv));
}

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

void init_sockaddr
(struct sockaddr_in *addr, const char  *str_port)
{
	unsigned short int port;

	port = (unsigned short int)get_num(str_port);

	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	(&(addr->sin_addr))->s_addr = INADDR_ANY;
}

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	struct timeval tv;
	int sd, ok, bytes_received;
	int dg_counter = 0, dg_total_size = 0;
	char buffer[dgram_size];
	socklen_t len;

	if(argc < 2) {
		fprintf(stderr, "Specify the port\n");
		return 1;
	}

	check_port_is_valid(argv[1]);
	daemonize_self();
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	init_sockaddr(&addr, argv[1]);
	set_timeout(&tv, timeout_sec, timeout_usec, sd);
	
	ok = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
	if(ok == -1) {
		perror("bind");
		return 1;
	}

	for(;;) {
		len = sizeof(addr);
		bytes_received = 
			recvfrom(
				sd,
				buffer,
				dgram_size,
				0,
				(struct sockaddr *)&addr,
				&len
			);

		if(len > 0) {
			dg_counter++;
			dg_total_size += bytes_received;
			len = sprintf(buffer, "%d %d", dg_counter, dg_total_size);
			ok = sendto(
				sd,
				buffer,
				len,
				0,
				(struct sockaddr *)&addr, sizeof(addr)
				);
			if(ok == -1)
				perror("Error sending datagram");
		}
	}

	close(sd);
	return 0;
}
