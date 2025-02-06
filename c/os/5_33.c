#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

enum { expected_argc = 2, buff = 64 };

int check_password(FILE *fd, char *input_password)
{
	char c;
	int flag = 0;

	while((c = getc(fd)) != '\n') {
		if(c != *input_password) {
			flag = 1;
			break;
		}
		input_password++;
	}
	if(!flag && *input_password != '\n')
		flag = 1;

	return flag;
}

int main(int argc, char **argv)
{
	struct termios ts1, ts2;
	char input_passwd[buff];
	FILE *fd;

	if(argc != expected_argc) {
		fprintf(stderr, "Specify only one argument: the filename\n");
		return 1;
	}

	if(!isatty(0)) {
		fprintf(stderr, "Not a terminal\n");
		return 2;
	}

	fd = fopen(argv[1], "r");
	if(fd == 0) {
		fprintf(stderr, "Can't open the file\n");
		return 3;
	}

	tcgetattr(0, &ts1);
	memcpy(&ts2, &ts1, sizeof(ts1));	/* save settings */
	ts1.c_lflag &= ~ECHO;			/* unset flag */
	tcsetattr(0, TCSANOW, &ts1);

	printf("Password: ");
	if(!fgets(input_passwd, buff, stdin)) {
		fprintf(stderr, "Unexpected end of file\n");
		fclose(fd);
		return 4;
	}

	tcsetattr(0, TCSANOW, &ts2);	/* default settings */

	if(check_password(fd, input_passwd)) {
		fprintf(stderr, "\nInvalid password\n");
		fclose(fd);
		return 5;
	}

	printf("\n");
	fclose(fd);

	return 0;
}
