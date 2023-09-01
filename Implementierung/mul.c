#include "mul.h"

int equal_length(unsigned char** a, unsigned char** b) {
    int len1 = strlen(*a);
    int len2 = strlen(*b);
    int dif = len2 - len1;

    //b longer than a
    if (dif > 0) {
       //allocate one byte for the terminated null byte
       unsigned char* tmp = malloc(sizeof(unsigned char) * (len2+1));
       if (tmp == NULL) {
           fprintf(stderr, "error occurs with malloc(), program aborts\n");
           exit(EXIT_FAILURE);
       }
       //set all the bits in between to zero
       memset(tmp, '0', dif);
       memcpy(tmp+dif, *a, len1+1);
       //change the string pointed by a
       *a = tmp;
       return len2;
    } else if (dif < 0) {
        dif = -dif;
       unsigned char* tmp = malloc(sizeof(unsigned char) * (len1+1));
       if (tmp == NULL) {
           fprintf(stderr, "error occurs with malloc(), program aborts\n");
           exit(EXIT_FAILURE);
       }
       memset(tmp, '0', dif);
       memcpy(tmp+dif, *b, len2+1);
       *b = tmp;
       return len1;
    }
    return len1;
}

unsigned char* big_add(unsigned char* a, unsigned char* b, _Bool free_disabled) {
    int len = equal_length(&a, &b);
    //allocate one byte to handle carry and one byte for the terminated null byte
    unsigned char* tmp = malloc(sizeof(unsigned char) * (len+2));
    if (tmp == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    *(tmp+len+1) = '\0';

    //indicate carry flag
    int carry = 0;
    //begin with the lowest digit
    for (int i = len-1; i >= 0; --i) {
        unsigned char fst = *(a+i) - '0';
        unsigned char sec = *(b+i) - '0';
        //increment the index once because of the length of the result string
        //using bit manipulation to calculate the sum
        *(tmp+i+1) = fst ^ sec ^ carry + '0';
        //using bit manipulation to calculate carry flag
        carry = (fst & sec) | (sec & carry) | (fst & carry);
    }
    //count all leading zero
    int cnt = 0;
    if (carry) {
        *tmp = '1';
        if(free_disabled == 0) {
            free(a);
            free(b);
        }
        return tmp;
    } else {
        *tmp = '0';
        //assume the result contains at least one effective bit
        //the bound is set to the second last digit
        for (int i = 0; i < len; ++i) {
            if (*(tmp+i) == '0') cnt += 1;
            else break;
        }
        //delete all leading zero
        unsigned char* res = malloc(sizeof(unsigned char) * (len+2-cnt));
        if (res == NULL) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        memcpy(res, tmp+cnt, len+2-cnt);
        free(tmp);
        if(!free_disabled) {
            free(a);
            free(b);
        }
        return res;
    }
}

//assume the return value of this function is always positive or zero
unsigned char* big_sub(unsigned char* a, unsigned char* b) {
    int len = equal_length(&a, &b);
    //allocate one byte for the terminated null byte
    unsigned char* tmp = malloc(sizeof(unsigned char) * (len+1));
    if (tmp == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    *(tmp+len) = '\0';

    //indicate borrow flag
    int borrow = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned char fst = *(a+i) - '0';
        unsigned char sec = *(b+i) - '0';
        //using bit manipulation to calculate the difference
        *(tmp+i) = fst ^ sec ^ borrow + '0';
        //using bit manipulation to calculate borrow flag
        borrow = (~fst & sec) | (~fst & borrow) | (sec & borrow);
    }
    //count leading zero
    int cnt = 0;
    //assume the result contains at least one effective bit
    //the bound is set to the second last digit
    for (int i = 0; i < len-1; ++i) {
        if (*(tmp+i) == '0') {
            cnt += 1;
        } else {
            break;
        }
    }
    unsigned char* res = malloc(sizeof(unsigned char) * (len+1-cnt));
    if (res == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    memcpy(res, tmp+cnt, len+1-cnt);
    free(tmp);
    return res;
}

unsigned char* mul_single_bit(unsigned char* a, unsigned char* b) {
    unsigned char ac = *a - '0';
    unsigned char bc = *b - '0';
    unsigned char* res = malloc(sizeof(unsigned char) * 2);
  
    if (res == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    *(res+1) = '\0';
    *res = ac * bc + '0';
    return res;
}

unsigned char* big_mul(unsigned char* x, unsigned char* y) {
    int idx_x = strlen(x);
    int idx_y = strlen(y);

    //handle the case that x or y evaluates to one or zero
    //recursion anker is the single bit which cannot be divided
    if (idx_x == 1) {
        if (idx_y == 1) {
            return mul_single_bit(x, y);
        }
    }
    //skip the last bit
    idx_x--;
    //iterate from the second last bit and stop when reach a non-zero
    while(idx_x--) {
        if (*(x+idx_x) == '1') break;
    }
    //the same with y
    idx_y--;
    while(idx_y--) {
        if (*(y+idx_y) == '1') break;
    }
    //-1 indicated the iteration reaches the most significant bit and they are all zero
    if (idx_x == -1) {
        idx_x = strlen(x);
        //determine the value of x is one or zero by only checking the last bit
        //do the rest multiplication accordingly
        if (*(x+idx_x-1) == '0') {
            unsigned char* zero = malloc(sizeof(unsigned char) * 2);
            if (zero == NULL) {
                fprintf(stderr, "error occurs with malloc(), program aborts\n");
                exit(EXIT_FAILURE);
            }
            *zero = '0';
            *(zero+1) = '\0';
            return zero;
        } else {
            idx_y  = strlen(y);
            int cnt = 0;
            for (int i = 0; i < idx_y-1; ++i) {
                if (*(y+i) == '0') cnt++;
                else break;
            }
            unsigned char* res = malloc(sizeof(unsigned char) * (idx_y+1-cnt));
            if (res == NULL) {
                fprintf(stderr, "error occurs with malloc(), program aborts\n");
                exit(EXIT_FAILURE);
            }
            memcpy(res, y+cnt, idx_y+1-cnt);
            return res;
        }
    }
    //the same with y
    if (idx_y == -1) {
        idx_y = strlen(y);
        if (*(y+idx_y-1) == '0') {
            unsigned char* zero = malloc(sizeof(unsigned char) * 2);
            if (zero == NULL) {
                fprintf(stderr, "error occurs with malloc(), program aborts\n");
                exit(EXIT_FAILURE);
            }
            *zero = '0';
            *(zero+1) = '\0';
            //free
            return zero;
        } else {
            idx_x = strlen(x);
            int cnt = 0;
            for (int i = 0; i < idx_x-1; ++i) {
                if (*(x+i) == '0') cnt++;
                else break;
            }
            unsigned char* res = malloc(sizeof(unsigned char) * (idx_x+1-cnt));
            memcpy(res, x+cnt, idx_x+1-cnt);
            return res;
        }
    }

    //make sure the both multiplier have the same length in the beginning each recursion call
    int len = equal_length(&x, &y);

    //divide the multiplier into two smaller one with the same length
    //formula:  x = 2^m * x1 + x0
    //          y = 2^m * y1 + y0
    //calculate the length which the multiplier needs to be divided into
    int n = len / 2;
    int m = len - n;

    //allocate one byte for the terminated null byte
    unsigned char* x1 = malloc(sizeof(unsigned char) * (n + 1));
    if (x1 == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    //divide the multiplier respectively
    memcpy(x1, x, n);
    *(x1 + n) = '\0';

    unsigned char* x0 = malloc(sizeof(unsigned char) * (m + 1));
    memcpy(x0, x + n, m);
    *(x0 + m) = '\0';
    if (x0 == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }

    unsigned char* y1 = malloc(sizeof(unsigned char) * (n + 1));
    memcpy(y1, y, n);
    *(y1 + n) = '\0';
    if (y1 == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }

    unsigned char* y0 = malloc(sizeof(unsigned char) * (m + 1));
    memcpy(y0, y + n, m);
    *(y0 + m) = '\0';
    if (y0 == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }

    //keep making recursive calls to calculate the production of each divided multiplier
    //x1y1      := x1 * y1
    unsigned char* x1y1 = big_mul(x1, y1);
    //x0y0      := x0 * y0
    unsigned char* x0y0 = big_mul(x0, y0);
    //x0x1y0y1  := (x0+x1) * (y0+y1)
    unsigned char* x0x1y0y1 = big_mul(big_add(x0, x1, 0), big_add(y0, y1, 0));

    //save the interim results so we can free the pointer after that
    //formula:  x * y = x0y0 + 2^n * ((x0+x1) * (y0+y1) - x0y0 - x1y1) + 2^2m * x1y1
    unsigned char* sub1 = big_sub(x0x1y0y1, x0y0);
    unsigned char* sub2 = big_sub(sub1, x1y1);
    unsigned char* shl = shift_left(sub2, m);
    free(x0x1y0y1);
    free(sub1);
    unsigned char* res = big_add(big_add(x0y0, shift_left(x1y1, 2 * m), 0), shl, 0);

    return res;
}

unsigned char* shift_left(unsigned char* a, int c) {
    //do nothing if the string to be shifted is zero
    if (*a == '0') {
        //constant zero in string representation
        unsigned char* zero = malloc(sizeof(unsigned char) * 2);
        if (zero == NULL) {
            fprintf(stderr, "error occurs with malloc(), program aborts\n");
            exit(EXIT_FAILURE);
        }
        *zero = '0';
        *(zero+1) = '\0';
        free(a);
        return zero;
    }
    int len = strlen(a);
    //allocate one byte for the terminated null byte
    unsigned char* res = malloc(sizeof(unsigned char) * (len+c+1));
    if (res == NULL) {
        fprintf(stderr, "error occurs with malloc(), program aborts\n");
        exit(EXIT_FAILURE);
    }
    *(res+len+c) = '\0';
    //moving the original number into the higher position of result
    memcpy(res, a, len);
    //fill the lower position with zero in order to achieve a left shift
    memset(res+len, '0', c);
    free(a);
    return res;
}