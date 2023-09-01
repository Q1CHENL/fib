#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

void INDUCTION_CHECK(int start, int end, _Bool v);
void print_test_result(_Bool passed);
_Bool fib_cmp(unsigned char* expected, unsigned char* provided, _Bool cross_check_enabled);
void FIB_N_CHECK(int n, _Bool v);
