//
// Created by Qichen Liu on 2022/6/25.
//
#include "fib_v0.h"
#include "cvt.h"
#include "mul.h"

//calculate the n-th fibonacci_number
void fib_v1(uint64_t n, size_t len, uint8_t buf[len]) {

    //base cases
    switch (n) {
        case 0 :
            *buf = 0;
            return;
        case 1 :
            *buf = 1;
            return;
        case 2 :
            *buf = 1;
            return;
        default :
            break;
    }

    //base matrix
    unsigned char *fib_matrix[4] = {"1", "1", "1", "0"};
    unsigned char *res[4];
    for (int i = 0; i < 4; i++) {
        res[i] = malloc(2);
        if (!res[i]) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //(0,0) of result matrix
    unsigned char * res0 = "1";
    //(0,1) of result matrix
    unsigned char * res1 = "1";
    //(1,0) of result matrix
    unsigned char * res2 = "1";
    //(1,1) of result matrix
    unsigned char * res3 = "0";

    //initialize result matrix
    memcpy(res[0], res0,2);
    memcpy(res[1], res1,2);
    memcpy(res[2], res2,2);
    memcpy(res[3], res3,2);
    //calculate fib
    while (n - 1 > 0) {
        unsigned char *zero = malloc(strlen(res[0]) + 1);
        if(!zero) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(zero, res[0]);

        unsigned char *one = malloc(strlen(res[1]) + 1);
        if(!one) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(one, res[1]);

        unsigned char *two = malloc(strlen(res[2]) + 1);
        if(!two) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(two, res[2]);

        unsigned char *three = malloc(strlen(res[3]) + 1);
        if(!three) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(three, res[3]);

        unsigned char *a[4] = {zero, one, two, three};

        unsigned char **tmp = matrix_mul(a, fib_matrix);
        for (int i = 0; i < 4; i++) {
            free(res[i]);
            res[i] = tmp[i];
        }
        free(zero);
        free(one);
        free(two);
        free(three);
        free(tmp);
        n--;
    }
    
    free(res[0]);
    free(res[2]);
    free(res[3]);
    _bitstring_to_bytes(res[1], buf, len);
    reverse(buf, len);

}


double fib_v1_benchmark(uint64_t n, size_t len, uint8_t buf[len]) {
    uint64_t ntmp = n;

    //benchmark
    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //base cases
    switch (n) {
        case 0 :
            *buf = 0;
            clock_gettime(CLOCK_MONOTONIC, &end);
            return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        case 1 :
            *buf = 1;
            clock_gettime(CLOCK_MONOTONIC, &end);
            return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        case 2 :
            *buf = 1;
            clock_gettime(CLOCK_MONOTONIC, &end);
            return end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        default :
            break;
    }

    //base matrix
    unsigned char *fib_matrix[4] = {"1", "1", "1", "0"};
    unsigned char *res[4];
    for (int i = 0; i < 4; i++) {
        res[i] = malloc(2);
        if (!res[i]) {
            
        }
    }
    
    //(0,0) of result matrix
    unsigned char * res0 = "1";
    //(0,1) of result matrix
    unsigned char * res1 = "1";
    //(1,0) of result matrix
    unsigned char * res2 = "1";
    //(1,1) of result matrix
    unsigned char * res3 = "0";

    //initialize result matrix
    memcpy(res[0], res0,2);
    memcpy(res[1], res1,2);
    memcpy(res[2], res2,2);
    memcpy(res[3], res3,2);

    //calculate fib
    while (n - 1 > 0) {

        //copy element of old matrix
        unsigned char *zero = malloc(strlen(res[0]) + 1);
        if(!zero) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(zero, res[0]);

        unsigned char *one = malloc(strlen(res[1]) + 1);
        if(!one) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(one, res[1]);

        unsigned char *two = malloc(strlen(res[2]) + 1);
        if(!two) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(two, res[2]);

        unsigned char *three = malloc(strlen(res[3]) + 1);
        if(!three) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        strcpy(three, res[3]);

        unsigned char *a[4] = {zero, one, two, three};

        //multiply base matrix
        unsigned char **tmp = matrix_mul(a, fib_matrix);
        //store result in res
        for (int i = 0; i < 4; i++) {
            free(res[i]);
            res[i] = tmp[i];
        }

        free(zero);
        free(one);
        free(two);
        free(three);
        free(tmp);
        n--;
    }

    //benchmark
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
    printf("%lu-th fibonacci number calculated after %f seconds\n", ntmp, time);

    free(res[0]);
    free(res[2]);
    free(res[3]);

    //convert and store result in buf
    _bitstring_to_bytes(res[1], buf, len);
    reverse(buf, len);

    return time;

}