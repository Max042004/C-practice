#include <math.h>
#include <stdint.h>
/*
 * FP16
 * 0bxxxxxxxxxxxxxxxx
 *   ||___||________|
 *   |  |       |
 *   1  |       10
 *   v  |       v
 * sign 5     mantissa
 *      v
 *   exponent
 */


static inline uint32_t  fp32_to_bits(float f)
{
	union {
		uint32_t as_bits;
		float as_value;
	} fp32 = {.as_value = f};
	return fp32.as_bits;
}




static inline float bits_to_fp32(uint32_t w)
{
	union {
		uint32_t as_bits;
		float as_value;
	} fp32 = {.as_bits = w};

	return fp32.as_value;
}



typedef uint16_t fp16_t;

static inline fp16_t fp32_to_fp16(float f)
{
	float scale_to_inf = 0x1.0p+112f;
	float scale_to_zero = 0x1.0p-110f;
	float base = (fabs(f)*scale_to_inf)*scale_to_zero;
	const uint32_t w = fp32_to_bits(f);
	const uint32_t shL1_w  = w + w;
	const uint32_t sign = w & UINT32_C(0x80000000);
	uint32_t bias = shL1_w & UINT32_C(0xFF000000);
	if (bias < UINT32_C(0x71000000)) bias = UINT32_C(0x71000000);
	//base = bits_to_fp32(bias >> 1) + UINT32_C();	
}

int main()
{
	float fp[5];
	fp[0] = 1;
	fp32_to_fp16(fp[0]);
	return 0;
}
