#include "item.h"
#include "strings.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

enum { item_size = 64 };
enum { read_error = -2 };
enum { max_number_pos = 11, ascii_numbers = 48 };

static int find_item(int fd, struct item_info *item)
{
	int position = 0;
	char buffer[item_size];
	int bytes_read;
	
	lseek(fd, 0, SEEK_SET);
	while((bytes_read = read(fd, buffer, item_size)) != 0) {
		if(bytes_read == -1) {
			return read_error;
		}
		if(compare_strings(item->name, buffer)) {
			return position * sizeof(struct item_info);
		}
		position++;
	}

	return -1;
}

static void fill_null(char *str, int pos, int max_pos)
{
	int i;
	for(i = pos; i <= max_pos; i++)
		str[i] = '\0';
}

void copy_itemname(char *dest, char *src)
{
	int i;

	for(i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];

	fill_null(dest, i, name_size);
}

int add_item(int fd, struct item_info *item)
{
	int position;
	int count;

	count = item->count;
	position = find_item(fd, item);
	if(position == read_error) {
		return read_error;
	}
	if(position == -1) {
		lseek(fd, 0, SEEK_END);
		write(fd, item, sizeof(struct item_info));
	} else {
		lseek(fd, position + name_size, SEEK_SET);
		read(fd, &(item->count), sizeof(item->count));
		lseek(fd, -sizeof(item->count), SEEK_CUR);
		item->count += count;
		write(fd, &(item->count), sizeof(item->count));
	}

	return 0;
}

int output_item(int fd, struct item_info *item)
{
	int position;
	int len;
	char count_ascii[max_number_pos];

	position = find_item(fd, item);
	if(position == read_error) {
		return read_error;
	}
	if(position == -1) {
		write(STDOUT_FILENO, "0\n", 2);
		return -1;
	}

	lseek(fd, position, SEEK_SET);
	read(fd, item, item_size);

	int_to_str(count_ascii, item->count);
	len = count_len(count_ascii);
	write(STDOUT_FILENO, count_ascii, len);
	write(STDOUT_FILENO, "\n", 1);

	return 0;
}

int output_table(int fd)
{
	int len, check_read;
	char count_ascii[max_number_pos];
	struct item_info item;

	lseek(fd, 0, SEEK_SET);

	while((check_read = read(fd, &item, item_size)) != 0) {
		if(check_read == -1) {
			return read_error;
		}
		len = count_len(item.name);
		write(STDOUT_FILENO, &(item.name), len);
		write(STDOUT_FILENO, " ", 1);
		int_to_str(count_ascii, item.count);
		len = count_len(count_ascii);
		write(STDOUT_FILENO, count_ascii, len);
		write(STDOUT_FILENO, "\n", 1);
	}

	return 0;
}

int join_tables(int in_fd1, int in_fd2, int out_fd)
{
	struct item_info *item;
	int status = 0, check_read;

	item = malloc(sizeof(struct item_info));

	lseek(in_fd1, 0, SEEK_SET);
	while((check_read = 
				read(in_fd1, item, sizeof(struct item_info))) != 0) {
		if(check_read == -1) {
			status = -1;
			goto ret;
		}
		add_item(out_fd, item);
	}

	lseek(in_fd2, 0, SEEK_SET);
	while((check_read = 
				read(in_fd2, item, sizeof(struct item_info))) != 0) {
		if(check_read == -1) {
			status = -1;
			goto ret;
		}
		add_item(out_fd, item);
	}

	ret:
		free(item);
		return status;
}