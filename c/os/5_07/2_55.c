#include "item.h"
#include "strings.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define ARGC_ERR "Provide file name, command and (for add/query) item name\n"
#define ARGC_ERR_LEN (sizeof(ARGC_ERR) - 1)
#define COMMAND_ERR "Unknown command"
#define COMMAND_ERR_LEN (sizeof(COMMAND_ERR) - 1)

void raise_argc_error()
{
	write(STDERR_FILENO, ARGC_ERR, ARGC_ERR_LEN);
	exit(1);
}

int handle_add(int argc, char **argv)
{
	int fd;
	struct item_info item;
	
	if(argc < 4)
		raise_argc_error();

	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror("Error opening file");
		close(fd);
		return 2;
	}

	copy_itemname(item.name, argv[3]);
	item.count = 1;
	if(add_item(fd, &item) == -2) {
		perror("Error reading file");
		return 2;
	}
	return 0;
}

int handle_query(int argc, char **argv)
{
	int fd;
	struct item_info item;
	
	if(argc < 4)
		raise_argc_error();
	
	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror("Error opening file");
		close(fd);
		return 2;
	}

	copy_itemname(item.name, argv[3]);
	item.count = 1;
	if(output_item(fd, &item) == -2) {
		perror("Error reading file");
		return 2;
	}

	return 0;
}

int handle_list(int argc, char **argv)
{
	int fd;
	
	if(argc < 3)
		raise_argc_error();
	
	fd = open(argv[1], O_RDWR);
	if(fd == -1) {
		perror("Error opening file");
		close(fd);
		return 2;
	}

	if(output_table(fd) == -2) {
		perror("Error reading file");
		return 2;
	}

	return 0;
}
int main(int argc, char **argv)
{
	int status;
	if(argc < 3)
		raise_argc_error();

	if(compare_strings(argv[2], "add")) {
		status = handle_add(argc, argv);
	}
	else if(compare_strings(argv[2], "query")) {
		status = handle_query(argc, argv);
	}
	else if(compare_strings(argv[2], "list")) {
		status = handle_list(argc, argv);
	}
	else {
		write(
			STDERR_FILENO,
			COMMAND_ERR,
			COMMAND_ERR_LEN
	    	);
		status = 3;
		return status;
	}

	return status;
}
