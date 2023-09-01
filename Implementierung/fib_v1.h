#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

void fib_v1(uint64_t n, size_t len, uint8_t buf[len]);
double fib_v1_benchmark(uint64_t n, size_t len, uint8_t buf[len]);

double fib_v1_mul_benchmark(uint64_t n, size_t len, uint8_t buf[len]);