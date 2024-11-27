typedef struct stack_of_digits {
	char data;
	struct stack_of_digits *prev, *next;
} sd;

typedef struct stack_of_operations {
	char data;
	struct stack_of_operations *prev, *next;
} so;

int main()
{
	enum { buffer = 1024 };
	char str[buffer];


}
