//
// Created by Qichen Liu on 2022/7/1.
//
#include "test.h"
#include "main.h"
#include "cvt.h"
#include "fib_numbers.h"
#include "mul.h"
#include "fib_v1.h"
#include "fib_v0.h"
#include "math.h"

#define SINGLE_TEST_UPPER_BOUND 1000

void INDUCTION_CHECK(int start, int end, _Bool v)
{
    if (start < 2)
    {
        printf("Invalid input: start index must be greater equal 2!\n");
        return;
    }
    int i = start;

    /* two previous numbers
     * n0: f(n-2)
     * n1: f(n-1)
     * */
    unsigned char *n0;
    unsigned char *n1;

    int len_buf1 = calc_length(i - 1);
    int len_buf2 = calc_length(i - 2);

    unsigned char *buf1 = malloc(len_buf1 + 1);
    *(buf1 + len_buf1) = '\0';
    unsigned char *buf2 = malloc(len_buf2 + 1);
    *(buf2 + len_buf2) = '\0';

    // calculate f(n-1) & f(n-2)
    fib_v1(i - 1, len_buf1, buf1);
    fib_v1(i - 2, len_buf2, buf2);

    // convert and store f(n-1) & f(n-2) in bufs
    reverse(buf1, len_buf1);
    unsigned char *dest1 = malloc(len_buf1 * 8 + 1);
    *(dest1 + len_buf1 * 8) = '\0';
    _bytes_to_bitstring(len_buf1, buf1, dest1);

    reverse(buf2, len_buf2);
    unsigned char *dest2 = malloc(len_buf2 * 8 + 1);
    *(dest2 + len_buf2 * 8) = '\0';
    _bytes_to_bitstring(len_buf2, buf2, dest2);

    // store f(n-1) & f(n-2) in n0 & n1 for later usage
    n0 = malloc(len_buf2 * 8 + 1);
    strcpy((char *)n0, (char *)dest2);
    n1 = malloc(len_buf1 * 8 + 1);
    strcpy((char *)n1, (char *)dest1);

    free(buf1);
    free(buf2);
    free(dest1);
    free(dest2);

    // number of passed tests
    int pass_sum = 0;
    for (; i <= end; i++)
    {

        printf("====================Verifying %d-th fibonacci number====================\n", i);
        int len_buf0 = calc_length(i);

        // calculate f(n)
        unsigned char *buf0 = malloc(len_buf0 + 1);
        *(buf0 + len_buf0) = '\0';

        // determine which implementation to use
        if (v == 0)
        {
            fib_v0(i, len_buf0, buf0);
        }
        else
        {
            fib_v1(i, len_buf0, buf0);
        }

        // convert and store f(n) in dest0
        reverse(buf0, len_buf0);
        unsigned char *dest0 = malloc(len_buf0 * 8 + 1);
        *(dest0 + len_buf0 * 8) = '\0';
        _bytes_to_bitstring(len_buf0, buf0, dest0);
        free(buf0);

        // store n1 to ntmp for later assignment to n0
        unsigned char *ntmp = malloc(strlen((char *)n1) + 1);
        *(ntmp + strlen((char *)n1)) = '\0';
        strcpy((char *)ntmp, (char *)n1);

        unsigned char *sum = big_add(n0, n1, 0);
        _Bool passed = fib_cmp(sum, dest0, 0);
        pass_sum += passed;

        // update n0: new n0 is the old n1, which is stored in ntmp
        n0 = malloc(strlen((char *)ntmp) + 1);
        strcpy((char *)n0, (char *)ntmp);
        free(ntmp);

        // update n1: new n1 is the new calculated f(n)
        n1 = malloc(len_buf0 * 8 + 1);
        *(n1 + len_buf0 * 8) = '\0';
        strcpy((char *)n1, (char *)dest0);
        free(dest0);

        print_test_result(passed);
    }

    // final result of the induction test
    if (pass_sum == end - start + 1)
    {
        printf("[%d/%d] All tests passed!\n", pass_sum, pass_sum);
    }
    else
    {
        printf("[%d/%d] %d tests passed, %d tests failed!\n", pass_sum, end - start + 1, pass_sum, start - end + 1 - pass_sum);
    }
}

// print test result
void print_test_result(_Bool passed)
{
    if (passed)
        printf("[SUCCESSFUL] Test passed!\n");
    else
        printf("[FAILED] Test failed!\n");
}

// test the n-th fibonacci number with n <=1000
void FIB_N_CHECK(int n, _Bool v)
{
    if (n > SINGLE_TEST_UPPER_BOUND)
    {
        printf("Single tests supports max. n=1000!");
        return;
    }

    int len_buf = calc_length(n);
    unsigned char *buf = malloc(len_buf + 1);
    *(buf + len_buf) = '\0';

    // determine implementation and calculate f(n)
    if (v == 0)
        fib_v0(n, len_buf, buf);
    else
        fib_v1(n, len_buf, buf);

    // convert and store f(n)
    reverse(buf, len_buf);
    unsigned char *dest = malloc(len_buf * 8 + 1);
    *(dest + len_buf * 8) = '\n';
    _bytes_to_bitstring(len_buf, buf, dest);
    free(buf);

    // check correctness of  f(n)
    unsigned char *dest_dec = malloc(strlen((char *)fib_numbers_1000[n]) + 1);
    cvt(dest, dest_dec, 2, 10);
    _Bool passed = fib_cmp(fib_numbers_1000[n], dest_dec, 0);
    free(dest_dec);
    print_test_result(passed);
}

// compare two string
_Bool fib_cmp(unsigned char *expected, unsigned char *provided, _Bool cross_check_enabled)
{
    if (!cross_check_enabled)
    {
        printf("Expected: %s\n", expected);
        printf("Provided: %s\n", provided);
    }
    else
    {
        printf("Result v0: %s\n", expected);
        printf("Result v1: %s\n", provided);
    }
    // the actual comparison
    int res = strcmp((char *)expected, (char *)provided);
    return res == 0 ? 1 : 0;
}

void CROSS_CHECK(int n)
{
    int len_buf = calc_length(n);

    // compute f(n) using v0
    unsigned char *buf0 = malloc(len_buf + 1);
    *(buf0 + len_buf) = '\0';
    fib_v0(n, len_buf, buf0);

    unsigned char *dest0 = malloc(len_buf * 8 + 1);
    *(dest0 + len_buf * 8) = '\n';
    _bytes_to_bitstring(len_buf, buf0, dest0);

    // compute f(n) using v1
    unsigned char *buf1 = malloc(len_buf + 1);
    *(buf1 + len_buf) = '\0';
    fib_v1(n, len_buf, buf1);

    unsigned char *dest1 = malloc(len_buf * 8 + 1);
    *(dest1 + len_buf * 8) = '\n';
    _bytes_to_bitstring(len_buf, buf1, dest1);

    // compare results of v0 & v1
    _Bool passed = fib_cmp(dest0, dest1, 1);
    print_test_result(passed);

    free(buf0);
    free(buf1);
    free(dest0);
    free(dest1);
}