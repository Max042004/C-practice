#include <stdlib.h>
#include <stdio.h>

struct node{
	int val, index;
};

int queue[200000] = {0};
struct node *stack[200000];

void pop(int stack_ptr)
{
	free(stack[stack_ptr]);
	stack[stack_ptr] = NULL;
}

void push(int stack_ptr, int index, int A_i)
{
	stack[stack_ptr] = malloc(sizeof(struct node));
	(stack[stack_ptr])->val = A_i;
	(stack[stack_ptr])->index = index;
}

int main()
{
	int length, A_i, nsd, q_i = 0;
	int stack_ptr = 0; // index
	scanf("%d\n", &length);
	for (int i = 0; i < length; i++){
		scanf("%d", &A_i);
		nsd = 0;
		while(stack_ptr >= 0){
			if (stack_ptr > 0 && (stack[stack_ptr-1])->val >= A_i) {
				pop(stack_ptr);
				stack_ptr--;
			}
			else {
				if (stack_ptr == 0) {
					nsd = -1;
				}
				else nsd = i - (stack[stack_ptr-1])->index;
				push(stack_ptr, i, A_i);
				stack_ptr++;
				break;
			}
		}
		printf("%d ", nsd);
		if (nsd != -1) queue[q_i++] = nsd;
	}
	printf("\n");
	if (queue[0] == 0){
		printf("EMPTY");
	}
	else{	
		for (int i = 0; i < q_i; i++){
			printf("%d ", queue[i]);
		}
	}
	return 0;
}
