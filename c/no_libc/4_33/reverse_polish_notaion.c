#include 

static int char_to_digit(char *c)
{
	return (int)c - 48;	
}

static void null_array(char *arr, int len)
{
	int i;
	for(i = 0; i < len; i++) {
		arr[i] = 0;
	}
}

static int check_math_expression(const char *expression)
{
	
}

void traslate_into_rpn(const char *expression, int len)
{
	char additional_array[len];
	char rpn_expr[len];
	int i;	

	null_arr(rpn_expr, len);

	for(i = 0; i < len; i++) {
		switch(expression[i]) {
			case:
				break;
			case:
				break;
			default:
				break;
		}
	}
}
