#include "../headers/conditions.h"
#include <stdio.h>

int is_end_of_string(char c)
{
	return c == '\n' || c == EOF || c == '\0';
}

int is_end_of_word(char c)
{
	return c == ' ' || c == '\0';
}

int is_space_char(char c)
{
	return c == ' ' || c == '\t';
}

int is_digit(char c)
{
	return c <= '9' && c >= '0';
}