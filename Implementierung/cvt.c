#include "cvt.h"

#define BUFFER_SIZE 300000

//reference: https://bearzpy.github.io/2016/10/15/C/
// %E5%A4%A7%E6%95%B0%E4%BB%BB%E6%84%8F%E8%BF%9B%E5%88%B6%E8%BD%AC%E6%8D%A2/index.html
int cvt(unsigned char* srcData, unsigned char* destData, int srcBase, int destBase) {

    char byteIndex[17] = "0123456789ABCDEF";
    unsigned char buffer[BUFFER_SIZE] = {0x00};
    unsigned char* pbuffer = buffer;
    unsigned char* psrc = srcData;
    unsigned char* pos = srcData;
    unsigned char* pdest = destData;
    int temp = 0;

    while (*psrc) {
        while (*pos) {
            temp *= srcBase;
            temp += value(*pos++);
            if (temp < destBase) {
                *pbuffer++ = '0';
                continue;
            }
            else {
                *pbuffer++ = byteIndex[(temp / destBase)];
                temp %= destBase;
            }
        }
        *pdest++ = byteIndex[temp];
        for (pos = buffer; pos < pbuffer && *pos == '0'; pos++);
        memcpy(psrc, pos, pbuffer - pos);
        psrc[pbuffer - pos] = 0;
        pos = psrc;
        pbuffer = buffer;
        temp = 0;
    }

    reverse(destData, pdest - destData);
    destData[pdest - destData] = '\0';
    return pdest - destData;
}

static int value(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    else
        return ch - 'A' + 10;
}

void reverse(unsigned char* src, int len) {
    int i = 0, j = len - 1;
    char temp;
    while (i < j) {
        temp = src[i];
        src[i++] = src[j];
        src[j--] = temp;
    }
}

/*
 * convert bitstring to bytes in memory
 * z.B. "10011111" -> 159(decimal) in one byte
 * used for storing result in buf
 * */
void _bitstring_to_bytes(uint8_t* src, uint8_t* dest, size_t len) {
    //bit string length
    int src_len = strlen(src);
    int rest = src_len % 8;
    for (int i = src_len - rest; i > 0; i -= 8) {
        int value = 0;
        for (int j = 0; j < 8; j++) {
            value += (src[i - j - 1 + rest] - '0') << j;
        }
        dest[len - 1] = value;
        len--;
    }
    if (rest != 0) {
        int value_rest = 0;
        for (int j = 0; j < rest; j++) {
            value_rest += (src[rest - j - 1] - '0') << j;
        }
        dest[0] = value_rest;
    }
}


/*
 * convert bytes in memory to bitstring
 * z.B. one byte of 159(decimal) -> "10011111"
 * used to output the result in memory
 * */
void _bytes_to_bitstring(int len, uint8_t* src, uint8_t* dest) {
    memset(dest, 0, strlen(dest));
    for (int i = 0; i < len; i++) {
        char tmp[9];
        char dest_tmp[9];
        sprintf(tmp, "%d", src[i]);
        cvt(tmp, dest_tmp, 10, 2);

        if (i > 0 && strlen(dest_tmp) < 8) {
            char dest_cmp_complete[9];
            int len_tmp = 8 - strlen(dest_tmp);
            char zeros[len_tmp + 1];
            memset(zeros, 48, len_tmp);
            zeros[len_tmp] = '\0';
            memset(dest_cmp_complete, 0, strlen(dest_cmp_complete));
            strcat(dest_cmp_complete, zeros);
            strcat(dest_cmp_complete, dest_tmp);
            strcat(dest, dest_cmp_complete);
            continue;
        }
        strcat(dest, dest_tmp);
    }
}
