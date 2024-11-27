int sys_write(int fd, const void *buf, int size);
int sys_read(int fd, void *buf, int size);

int main()
{
	enum { buffer = 1024 };
	char arr[buffer];
	sys_read(0, arr, 1);
	sys_write(1, arr, 1);

	return 0;
}
