# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>
# include <time.h>

void fib_v0(uint64_t n, size_t len, uint8_t buf[len]);
double fib_v0_benchmark(uint64_t n, size_t len, uint8_t buf[len]);
unsigned char** matrix_mul(unsigned char** a, unsigned char** b);