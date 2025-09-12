/* Stack 
  Takes a series of character as input,
  then push "{, [, (" and pop "}, ], )" on stack.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int stack_size = 100;

void print_stack(char *stack_bottom)
{
	if(*stack_bottom == 0) printf("empty stack\n");
	else printf("bottom: %s", stack_bottom);
}

// pop one element from stack
bool pop(char *str_ptr, char **stack_pointer, char *stack_bottom)
{
	printf("pop: %c\n", *str_ptr);
	if(*stack_pointer - stack_bottom <= 0){
		printf("error, stack is empty\n");
		return false;
	}
	switch(*str_ptr){
		case 41: 
			if(*(*stack_pointer-1) != 40){
				printf("error, no ) on top of stack\n");
				return false;
			}
			break;

		case 93:
			if(*(*stack_pointer-1) != 91){
				printf("error, no ] on top of stack top\n");
				return false;
			}
			break;

		case 125:
			if(*(*stack_pointer-1) != 123){
				printf("error, no } on top of stack a\n");
				return false;
			}
			break;
	}
	*stack_pointer = *stack_pointer - 1;
	**stack_pointer = 0;
	print_stack(stack_bottom);
	printf("\n\n");

	return true;
}

// push one element to stack from a array
bool push(char *str_ptr, char **stack_pointer, char *stack_bottom)
{
	if(*stack_pointer - stack_bottom >= 100){
		printf("error, stack is full\n");
		return false;
	}
	printf("push: %c\n", *str_ptr);
	memcpy(*stack_pointer, str_ptr, 1);
	*stack_pointer = *stack_pointer + 1;
	print_stack(stack_bottom);
	printf("\n\n");
	return true;
}

int main()
{
	char *stack_bottom = malloc(stack_size); // 100 bytes
	for(int i = 0; i < 100; i++){
		stack_bottom[i] = '\0';
	}
	char *stack_pointer = stack_bottom;
	char str[100];
	scanf("%s", str);
	char *str_ptr = &str[0];
	for(;;){
		if(*str_ptr == 0) break;
		if(*str_ptr == 123 || *str_ptr == 91 || *str_ptr == 40){
			if(!push(str_ptr, &stack_pointer, stack_bottom)) break;
		}
		if(*str_ptr == 125 || *str_ptr == 93 || *str_ptr == 41){
			if(!pop(str_ptr, &stack_pointer, stack_bottom)) break;
		}
		str_ptr = str_ptr + 1;
	}

	free(stack_bottom);

	return 0;
}
