#include <stdio.h>

void e(int f)
{
	return;
}

void (*e_ptr)(int) = e;

char b(char **g, char *h)
{
	char c = 'c';
	return c;
}

void (*func(int a, char (*b)(char **, char *)))(int)
{
	printf("a = %d, b points to address %p", a, b);
	return e_ptr;
}


int main()
{
	void(*(*func_ptr)(int, char (*)(char **, char *)))(int) = func;
	func_ptr(1, &b);

	return 0;
}
