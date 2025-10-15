#include <stdio.h>

int main()
{
	int i = 0;

	/* undefined behavior
	 * GCC: 2 1 0
	 * LLVM: 0 1 2
	 * Each compiler will have different implementation for standard library
	 */	
	printf("%d %d %d\n", i++, i++, i++);
	printf("%d", i);
}
