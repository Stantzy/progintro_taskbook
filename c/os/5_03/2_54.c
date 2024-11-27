#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

enum { name_length = 256, buf_size = 4096 };

struct bin_info {
	char filename[name_length];
	int int_count;
	int max;
	int min;
};

int analyze_bin(struct bin_info *b_inf, int fd_bin, const char *filename)
{
	const char *tmp;
	int buffer[buf_size];
	int readed;
	int i;

	b_inf->int_count = 0;

	tmp = filename;
	for(i = 0; tmp[i] != '\0'; i++)
		b_inf->filename[i] = tmp[i];

	b_inf->filename[i] = '\0';

	readed = read(fd_bin, buffer, buf_size);
	if(readed == -1)
		return 1;
	
	readed /= 4;
	b_inf->max = buffer[0];
	b_inf->min = buffer[0];

	for(i = 0; i < readed; i++) {
		if(buffer[i] > b_inf->max) // error here replace buffer[]i
			b_inf->max = buffer[i];
		else
		if(buffer[i] < b_inf->min) // same
			b_inf->min = buffer[i];
	}

	b_inf->int_count = readed;

	return 0;	
}

void reverse_arr(char *start, char *end)
{
	while(start < end) {
		char tmp = *start;
		*start++ = *end;
		*end-- = tmp;
	}	
}

void int_to_str(char *str, int number)
{
	int i = 0;
	int is_negative = 0;

	if(number == 0) {
		str[i] = 0;
		i++;
	}

	if(number < 0) {
		number *= -1;
		is_negative = 1;
	}

	while(number != 0) {
		str[i] = (number % 10) + 48;
		number /= 10;
		i++;
	}

	if(is_negative) {
		str[i] = '-';
		i++;
	}

	str[i] = '\0';
	reverse_arr(str, str+i-1);
}

int count_length(char *str)
{
	char *tmp = str;
	int counter = 0;
	
	while(*tmp != '\0') {
		counter++;
		tmp++;
	}

	return counter;
}

void fill_null(char *arr, int len)
{
	int i;
	for(i = 0; i < len; i++) {
		arr[i] = 0;
	}
}

int record_bin_info(int fd_txt, struct bin_info *b_inf)
{
	enum { buffer = 64 };
	char number[buffer];
	int len;

	len = count_length(b_inf->filename);
	write(fd_txt, b_inf->filename, len);
	write(fd_txt, "\n", 1);

	int_to_str(number, b_inf->int_count);
	len = count_length(number);
	write(fd_txt, number, len);
	write(fd_txt, "\n", 1);
	fill_null(number, buffer);

	int_to_str(number, b_inf->max);
	len = count_length(number);
	write(fd_txt, number, len);
	write(fd_txt, "\n", 1);
	fill_null(number, buffer);

	int_to_str(number, b_inf->min);
	len = count_length(number);
	write(fd_txt, number, len);
	write(fd_txt, "\n\0", 2);

	return 0;
}

int main(int argc, char **argv)
{
	struct bin_info *b_inf;
	int fd_bin, fd_new;
	int status = 0;
	int i;

	if(argc < 3) {
		perror(
			"Provide the names of the binary files "
			"and the text file to output\n"
		);
		return 1;
	}

	b_inf = malloc(sizeof(struct bin_info));
	*(b_inf->filename) = '\0';

	// last argv is .txt file to be created
	fd_new = open(argv[argc - 1], O_CREAT | O_WRONLY, 0666);
	if(fd_new == -1) {
		perror("Can't create new .txt file\n");
		free(b_inf);
		close(fd_new);
		return 2;
	}

	// work with .bin files
	for(i = 1; i < argc - 1; i++) {
		int check = 0;

		fd_bin = open(argv[i], O_CREAT | O_RDONLY, 0666);
		if(fd_bin == -1) {
			perror("Can't create new .bin file\n");
			status = 2;
			goto endprog;
		}
		
		check = analyze_bin(b_inf, fd_bin, argv[i]);
		if(check) {
			perror("File analysis error");
			status = 3;
			goto endprog;
		}

		check = record_bin_info(fd_new, b_inf);
		if(check) {
			perror("Error writing to the file");
			status = 4;
			goto endprog;
		}
	}

	endprog:
		free(b_inf);
		close(fd_bin);
		close(fd_new);
		return status;
}
