/*
 * For FP32: 0bXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 *             ||______||_____________________|
 *             1    8             23
 *             |    |              |
 *             v    v              v
 *            sign exponent     mantissa
 * 
 * Zero:         biased exponent = 0 , mantissa = 0
 * Normalized:   0 < biased exponent < 255    value = (-1)^sign * (1.mantissa) * 2^(biased exponent - 127)
 * Denormalized: biased exponent = 0          value = (-1)^sign * (0.mantissa) * 2^(-126)
 * Infinity:     biased exponent = 255
 * NaN:          biased exponent = 255, mantissa != 0
 */
