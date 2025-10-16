#include <stdio.h>

#define MAXA 1000000
int arr[MAXA+5] = {0};
int prefix[MAXA+5] = {0};

int main()
{
	int n = 0, k = 0, t = 0, a = 0, f = 0, a_max = 0, max_t = 0;
	long long f_bigg = 0;
	scanf("%d %d", &n, &k);
	for(int i = 0; i < n; i++){
		scanf("%d", &a);
		if (a > a_max) a_max = a;
		arr[a]++;
	}

	for(int i = 1; i <= a_max; i++){
		prefix[i] = prefix[i-1] + arr[i];
	}

	t = k;	
	while (t <= a_max){
		long long s = 0;
		int kbin = 1;
		for (int L = t; L <= a_max; L+= t, ++kbin){
			int R = L+t-1;
			if (R > a_max) R = a_max;
			int tmp = prefix[R] - prefix[L-1];
			s += 1LL*kbin*tmp;
		}
		long long F = s*t;
		if (F > f_bigg) {
			f_bigg = F;
			max_t = t;
		}
		t++;
		f = 0;
	}
	printf("%lld", f_bigg);
	return 0;
}
