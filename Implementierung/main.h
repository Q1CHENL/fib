#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

void print_help_message(char* program);
size_t calc_length(int n);
double handler(int imp, int bmk, int ite, int n, size_t len, uint8_t* buf);