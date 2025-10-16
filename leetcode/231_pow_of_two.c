#include <stdio.h>
#include <stdbool.h>

// one bit 1
bool isPowerOfTwo(int n, int *k)
{
	if (n <= 0) return false;

	int a, b;
	unsigned m = n & 0x7FFFFFFF;
	unsigned shift = 16, fb = 0;
	while (shift != 0){
		a = m >> shift;
		if (a) {
			m = a;
			fb += shift;
		}
		shift >>= 1;
	}
	if (n - (0x1 << fb) != 0) return false;

	*k = fb;
	return true;
}


int main()
{
	int c, k = 0;
	scanf("%d", &c);
	
	printf("%d\n", isPowerOfTwo(c, &k));
	printf("%d\n", k);
	return 0;
}
