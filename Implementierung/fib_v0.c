#include "fib_v0.h"
#include "mul.h"
#include "cvt.h"

void fib_v0(uint64_t n, size_t len, uint8_t buf[len]) {
    //in case n = 0 or 1 or 2;
    switch(n) {
        case 0 : *buf = 0; return;
        case 1 : *buf = 1; return;
        case 2 : *buf = 1; return;
        default : break;
    }
    
    unsigned char* result;
    //current result as fibmatrix form
    unsigned char* res[4];
    //current exponent of fibmatrix
    unsigned char* fibmatrix[4];
    //check if malloc returns a null pointer
    for(int i = 0; i < 4; i++) {
        if ((res[i] = (unsigned char*) malloc(2)) == NULL || (fibmatrix[i] = (unsigned char *) malloc(2)) == NULL){
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
    }
   
    //(0,0) of identity matrix
    unsigned char * res0 = "1";
    //(0,1) of identity matrix
    unsigned char * res1 = "0";
    //(1,0) of identity matrix
    unsigned char * res2 = "0";
    //(1,1) of identity matrix
    unsigned char * res3 = "1";

    //initialize result matrix
    memcpy(res[0], res0,2);
    memcpy(res[1], res1,2);
    memcpy(res[2], res2,2);
    memcpy(res[3], res3,2);
   
    //(0,0) of fibmatrix
    unsigned char* zero = "1";
    //(0,1) of fibmatrix
    unsigned char* one = "1";
    //(1,0) of fibmatrix
    unsigned char* two = "1";
    //(1,1) of fibmatirx
    unsigned char* three = "0";
   
    memcpy(fibmatrix[0], zero, 2);
    memcpy(fibmatrix[1], one, 2);
    memcpy(fibmatrix[2], two, 2);
    memcpy(fibmatrix[3], three, 2);
   
    int firstbit = n%2;
    n >>= 1;
    int bit;
    while (n > 0) {
        //value of current bit
        bit = n%2;
        unsigned char** temp = matrix_mul(fibmatrix, fibmatrix);
        for(int i = 0; i < 4; i++) {
            fibmatrix[i] = temp[i];
        }
        free(temp);
        if (bit) {
            temp = matrix_mul(res, fibmatrix);
            for(int i = 0; i < 4; i++) {
                res[i] = temp[i];
            }
            free(temp);
        }
        n >>= 1;
    }
    if (firstbit) {
        //result = fib(n + 1)
        result = res[0];
        for (int i = 1; i < 4; i++) {
            free(res[i]);
            free(fibmatrix[i]);
        }
        free(fibmatrix[0]);
    }else {
        //result = fib(n)
        result = res[1];
        for(int i = 2; i < 4; i++) {
            free(res[i]);
            free(fibmatrix[i]);
        }
        free(res[0]);
        free(fibmatrix[0]);
        free(fibmatrix[1]);
    }
    _bitstring_to_bytes(result, buf, len);
    reverse(buf,len);
}

/*
    |a0, a1| * |b0, b1|
    |a2, a3|   |b2, b3|
    = a0b0 + a1b2, a0b1 + a1b3
      a2b0 + a3b2, a2b1 + a3b3
*/
unsigned char** matrix_mul(unsigned char** a, unsigned char** b) {
    unsigned char* a0b0 = big_mul (a[0], b[0]);
    unsigned char* a1b2 = big_mul (a[1], b[2]);
    
    unsigned char* a0b1 = big_mul (a[0], b[1]);
    unsigned char* a1b3 = big_mul (a[1], b[3]);

    unsigned char* a2b0 = big_mul (a[2], b[0]);
    unsigned char* a3b2 = big_mul (a[3], b[2]);

    unsigned char* a2b1 = big_mul (a[2], b[1]);
    unsigned char* a3b3 = big_mul (a[3],b[3]);

    unsigned char** res = (unsigned char**) malloc(sizeof(unsigned char*) * 4);
    if (res == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    res[0] = big_add(a0b0, a1b2, 0);
    res[1] = big_add(a0b1, a1b3, 0);
    res[2] = big_add(a2b0, a3b2, 0);
    res[3] = big_add(a2b1, a3b3, 0);

    return res;
}

double fib_v0_benchmark(uint64_t n, size_t len, uint8_t buf[len]) {
    uint64_t ntmp = n;
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //in case n = 0 or 1 or 2;
    switch(n) {
        case 0 : *buf = 0; clock_gettime(CLOCK_MONOTONIC, &end); return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        case 1 : *buf = 1; clock_gettime(CLOCK_MONOTONIC, &end); return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        case 2 : *buf = 1; clock_gettime(CLOCK_MONOTONIC, &end); return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        default : break;
    }
    unsigned char* result;
    //current result as fibmatrix form
    unsigned char* res[4];
    //current exponent of fibmatrix
    unsigned char* fibmatrix[4];
    //check if malloc returns a null pointer
    for(int i = 0; i < 4; i++) {
        if ((res[i] = (unsigned char*) malloc(2)) == NULL || (fibmatrix[i] = (unsigned char *) malloc(2)) == NULL){
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
    }

    unsigned char * res0 = "1";
    unsigned char * res1 = "0";
    unsigned char * res2 = "0";
    unsigned char * res3 = "1";
    //initialize result matrix
    memcpy(res[0], res0,2);
    memcpy(res[1], res1,2);
    memcpy(res[2], res2,2);
    memcpy(res[3], res3,2);

    //(0,0) of fibmatrix
    unsigned char* zero = "1";
    //(0,1) of fibmatrix
    unsigned char* one = "1";
    //(1,0) of fibmatrix
    unsigned char* two = "1";
    //(1,1) of fibmatirx
    unsigned char* three = "0";

    memcpy(fibmatrix[0], zero, 2);
    memcpy(fibmatrix[1], one, 2);
    memcpy(fibmatrix[2], two, 2);
    memcpy(fibmatrix[3], three, 2);

    int firstbit = n%2;
    n >>= 1;
    int bit;
    while (n > 0) {
        //value of current bit
        bit = n%2;
        unsigned char** temp = matrix_mul(fibmatrix, fibmatrix);
        for(int i = 0; i < 4; i++) {
            fibmatrix[i] = temp[i];
        }
        free(temp);
        if (bit) {
            temp = matrix_mul(res, fibmatrix);
            for(int i = 0; i < 4; i++) {
                res[i] = temp[i];
            }
            free(temp);
        }
        bit = n%2;
        n >>= 1;
    }
    if (firstbit) {
        //result = fib(n + 1)
        result = res[0];
        for (int i = 1; i < 4; i++) {
            free(res[i]);
            free(fibmatrix[i]);
        }
        free(fibmatrix[0]);
    }else {
        //result = fib(n)
        result = res[1];
        for(int i = 2; i < 4; i++) {
            free(res[i]);
            free(fibmatrix[i]);
        }
        free(res[0]);
        free(fibmatrix[0]);
        free(fibmatrix[1]);
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("%lu-th fibonacci number calculated after %f seconds\n",ntmp, time);
    _bitstring_to_bytes(result, buf, len);
    reverse(buf,len);

    return time;
}