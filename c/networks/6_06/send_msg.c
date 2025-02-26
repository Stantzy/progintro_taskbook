#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

enum {
	num_max_len = 4,
	ip_max_dots = 3,
	ip_nums = 4,
	next_ip_num = 8,
	min_ip_num = 0,
	max_ip_num = 255
};

enum {
	timeout = 5,
	dgram_size = 508
};

enum {
	min_port_value = 1024,
	max_port_value = 49151
};

void exit_with_error()
{
	fprintf(stderr, "Invalid ip address\n");
	exit(EXIT_FAILURE);
}

unsigned int get_num_from_ip(const char **src)
{
	char str_num[num_max_len] = { 0, 0, 0, 0 };
	int i, mult = 1;
	unsigned int number = 0;

	if(**src == '.')
		(*src)++;

	for(
		i = 0;
		i < num_max_len && **src != '.' && **src != '\0';
		i++, (*src)++
	) {
		str_num[i] = **src;
	}

	if(i > num_max_len - 1)	/* num len > 3 */
		exit_with_error();

	for(i = num_max_len - 1; i >= 0; i--) {
		if(str_num[i] == 0)
			continue;

		number += (str_num[i] - '0') * mult;
		mult *= 10;	/* next number position */
	}

	*src = &(**src);

	if(number > max_ip_num)
		exit_with_error();

	return (unsigned int)number;
}

void check_ip_is_valid(const char *ip_str)
{
	int count_dots = 0;
	const char *tmp = ip_str;

	while(*tmp != '\0') {
		if(*tmp == '.')
			count_dots++;
		tmp++;
	}

	if(count_dots != ip_max_dots)
		exit_with_error();

	tmp = ip_str;
	while(*tmp != '\0')
		get_num_from_ip(&tmp);
}

unsigned int get_ip_as_num(const char *ip_str)
{
	int i;
	unsigned int int_ip = 0;

	for(i = 0; i < ip_nums; i++) {
		int_ip += (unsigned int)get_num_from_ip(&ip_str);
		if(i != ip_nums - 1)
			int_ip <<= next_ip_num;
	}

	return int_ip;
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
(struct sockaddr_in *addr, unsigned int h_ip, unsigned short int h_port)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons(h_port);
	(&(addr->sin_addr))->s_addr = htonl(h_ip);
}

void check_result(int res, const char *func_name)
{
	if(res == -1) {
		perror(func_name);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char **argv)
{
	struct sockaddr_in addr;
	int sockfd, bytes_sent;
	unsigned int h_ip = 0;
	unsigned short int h_port = 0;
	socklen_t len;

	if(argc < 4) {
		fprintf(stderr, "Specify IP, port and message\n");
		return 1;
	}	

	check_ip_is_valid(argv[1]);
	h_ip = get_ip_as_num(argv[1]);

	h_port = get_port_as_num(argv[2]);
	check_port_is_valid(h_port);

	init_sockaddr(&addr, h_ip, h_port);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	check_result(sockfd, "socket");

	len = sizeof(addr);
	bytes_sent =
	sendto(
		sockfd,
		argv[3],
		strlen(argv[3]),
		0,
		(const struct sockaddr *)&addr,
		len
	);
	check_result(bytes_sent, "sendto");

	close(sockfd);
	return 0;
}
