#include <stdio.h>

void check_str_literal(char *str)
{
	char *tmp_str_literal = str;
	//tmp_str_literal[2] = 81;
	return;
}

int main()
{
	int a[][4];
	int (*a_ptr_array)[4];
	char c[4] = {'a', 'b', 'c', 'd'};
	char *c_ptr = &c[0];

	char str_array[] = "No, not point to string literal";
	str_array[2] = 81;
	printf("%s\n", str_array);
	char *str_literal = "Yes, point to string literal";
	char str_stack[] = {'H', 'e', 'l', 'l', 'o', '\0'};
	//str_literal[2] = 81;
	check_str_literal("World");

	return 0;
}
