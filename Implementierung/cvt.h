#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

int cvt(unsigned char* srcData, unsigned char* destData, int srcBase, int destBase);
static int value(char ch);
void reverse(unsigned char* src, int len);
void _bitstring_to_bytes(uint8_t* src, uint8_t* dest, size_t len);
void _bytes_to_bitstring(int len, uint8_t* src, uint8_t* dest);
