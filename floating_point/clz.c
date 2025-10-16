#include <stdint.h>
#include <stdio.h>

static inline unsigned clz(uint32_t x)
{
    int n = 32, c = 16;
    do {
        uint32_t y = x >> c;
        if (y) {
            n -= c;
            x = y;
        }
        c >>= 1;
    } while (c);
    return n - x; // after interation, x still have one last bit not count, so n = n-x
}

int main()
{
	uint32_t x = 0x00000001;
	printf("%d\n", clz(x));
	return 0;
}
