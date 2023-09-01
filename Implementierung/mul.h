#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int equal_length(unsigned char** a, unsigned char** b);
unsigned char* big_add(unsigned char* a, unsigned char* b, _Bool free_disabled);
unsigned char* big_sub(unsigned char* a, unsigned char* b);
unsigned char* big_mul(unsigned char* x, unsigned char* y);
unsigned char* mul_single_bit(unsigned char* a, unsigned char* b);
unsigned char* shift_left(unsigned char* a, int c);
