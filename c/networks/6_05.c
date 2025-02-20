#include <stdio.h>
#include <stdlib.h>

enum {
	num_max_len = 4,
	ip_max_dots = 3,
	ip_nums = 4,
	ip_bits = 32,
	subnets_count = 32,
	min_ip_num = 0,
	max_ip_num = 255
};

void exit_with_error()
{
	fprintf(stderr, "Invalid ip address\n");
	exit(1);
}

unsigned char get_num_from_ip(char **src)
{
	char str_num[num_max_len] = { 0, 0, 0, 0 };
	int i, mult = 1;
	int number = 0;

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

	return (unsigned char)number;
}

int check_ip_is_valid(char *ip_str)
{
	int count_dots = 0;
	char *tmp = ip_str;

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


	return 0;
}

void get_ip_as_nums(char *ip_str, unsigned char ip[ip_nums])
{
	int i;
	for(i = 0; i < ip_nums; i++) {
		ip[i] = get_num_from_ip(&ip_str);
	}
}

void print_cur_subnet(unsigned char ip[ip_nums], int subnet)
{
	printf(
		"%u.%u.%u.%u/%u\n",
		ip[0],
		ip[1],
		ip[2],
		ip[3],
		subnet
	);
}

void print_subnets(unsigned char ip[ip_nums])
{
	int i, pos;
	unsigned char ip_tmp[ip_nums];
	unsigned char mask = 0b11111111;

	for(i = 0; i < ip_nums; i++)
		ip_tmp[i] = ip[i];

	print_cur_subnet(ip_tmp, ip_bits);
	for(i = 1; i <= ip_bits; i++) {
		mask <<= 1;
		pos = ip_bits - i;

		if(pos >= 24 && pos < ip_bits && ip_tmp[3] != 0)
			ip_tmp[3] &= mask;
		else if(pos >= 16 && pos < 24 && ip_tmp[2] != 0)
			ip_tmp[2] &= mask;
		else if(pos >= 8 && pos < 16 && ip_tmp[1] != 0)
			ip_tmp[1] &= mask;
		else if(pos < 8 && ip_tmp[0] != 0)
			ip_tmp[0] &= mask;
		
		print_cur_subnet(ip_tmp, pos);
		
		pos--;
		if(mask == 0b00000000)
			mask = 0b11111111;
	}
}

int main(int argc, char **argv)
{
	char *ip_str;
	unsigned char ip[ip_nums];

	if(argc < 2) {
		fprintf(stderr, "Specify the ip address\n");
		return 1;
	}

	ip_str = argv[1];
	
	check_ip_is_valid(ip_str);
	get_ip_as_nums(ip_str, ip);
	print_subnets(ip);

	return 0;
}
