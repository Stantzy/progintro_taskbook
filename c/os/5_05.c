#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

enum { max_len = 4096, buf_size = 4096 };

int count_length(const char *str)
{
	int i = 0;
	while(str[i] != '\0')
		i++;
	return i;
}

int str_to_number(const char *str)
{
	enum { ascii_code = 48 };
	int length, tmp, i;
	int number = 0;
	int mul = 1;

	length = count_length(str);
	for(i = 1; i <= length; i++) {
		tmp = (str[length - i] - ascii_code) * mul;
		mul *= 10;			// next number position
		number += tmp;
	}

	return number;
}

void fill_arr(char *arr, int length, char c)
{
	int i;
	for(i = 0; i < length; i++)
		arr[i] = c;
}

int main(int argc, char **argv)
{
	int fd, start_position, len;
	char buffer[buf_size];
	unsigned char value;

	if(argc != 5) {
		perror("Specify filename, start pos, length and byte value");
		return 1;
	}

	fd = open(argv[1], O_CREAT | O_WRONLY, 0666);
	if(fd == -1) {
		perror("Can't open the file");
		return 2;
	}

	start_position = str_to_number(argv[2]);
	len = str_to_number(argv[3]);
	value = (char)str_to_number(argv[4]);

	lseek(fd, start_position, SEEK_SET);
	fill_arr(buffer, len < max_len ? len : max_len, value);

	while(len > 0)
		len -= write(fd, buffer, len < max_len ? len : max_len);

	close(fd);
	return 0;
}
