#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <coreMd5.h>

const bit32 t[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

const int s[] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

bit32 rol(bit32 val, int size) {
    bit32 res = val << size;
    res |= val >> (32 - size);
    return res;
}

void md5_in(bit32 *A, bit32 *B, bit32 *C, bit32 *D, unsigned int *temp) {
    bit32 a = *A, b = *B, c = *C, d = *D;
    bit32 f;
    int g;

    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
        }
        int tmp = d;
        d = c;
        c = b;
        b = b + rol(a + f + t[i] + temp[g], s[i]);
        a = tmp;
    }

    *A += a;
    *B += b;
    *C += c;
    *D += d;
}

char *md5(char *data, int bitLen) {
    bit32 A = 0x67452301;
    bit32 B = 0xEFCDAB89;
    bit32 C = 0x98BADCFE;
    bit32 D = 0x10325476;

    unsigned int temp[16] = {0};
    int temp_index = 0, data_index = 0;

    while(data[data_index] != '\0') {
        for (; temp_index < 64 && data[data_index] != '\0'; temp_index++, data_index++)
            ((char *)&temp[temp_index / 4])[temp_index % 4] = data[data_index];
        if (temp_index == 64) {
            temp_index = 0;
            md5_in(&A, &B, &C, &D, temp);
        }
    }
    ((char *)&temp[temp_index / 4])[temp_index++ % 4] = 0x80;
    if (temp_index > 56) {
        for (; temp_index < 64; temp_index++)
            ((char *)&temp[temp_index / 4])[temp_index % 4] = 0x00;
        temp_index = 0;
        md5_in(&A, &B, &C, &D, temp);
    }
    for (; temp_index < 64; temp_index++)
        ((char *)&temp[temp_index / 4])[temp_index % 4] = 0x00;
    temp[14] = bitLen * 8;
    md5_in(&A, &B, &C, &D, temp);
    
    char *res = (char *)malloc(sizeof(char) * 33);
    sprintf(res, "%08X%08X%08X%08X", htonl(A), htonl(B), htonl(C), htonl(D));
    return res;

    // lastest turn
    /*for (; temp_index < 56 && data[data_index] != '\0'; temp_index++, data_index++)
        ((char *)&temp[temp_index / 4])[temp_index % 4] = data[data_index];
    ((char *)&temp[temp_index / 4])[temp_index % 4] = 0x80;
    temp_index++;
    for (; temp_index < 56; temp_index++)
    ((char *)&temp[temp_index / 4])[temp_index % 4] = 0x00;
    // 长度bug
    temp[14] = bitLen * 8;
    for (int i = 0; i < 16; i++)
        printf("%08X ", temp[i]);
    printf("\n");

    bit32 a = A, b = B, c = C, d = D;
    bit32 f;
    int g;
    printf("%08X%08X%08X%08X\n", a, b, c, d);

    for (int i = 0; i < 64; i++) {
        if (i < 16) {
            f = (b & c) | ((~b) & d);
            g = i;
        } else if (i < 32) {
            f = (d & b) | ((~d) & c);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = b ^ c ^ d;
            g = (3 * i + 5) % 16;
        } else {
            f = c ^ (b | (~d));
            g = (7 * i) % 16;
        }
        int tmp = d;
        d = c;
        c = b;
        b = b + rol(a + f + t[i] + temp[g], s[i]);
        a = tmp;
        printf("%08X %08X %08X %08X\n", a, b, c, d);
    }

    A = a + A;
    B = b + B;
    C = c + C;
    D = d + D;

    char *res = (char *)malloc(sizeof(char) * 33);
    sprintf(res, "%08X%08X%08X%08X", htonl(A), htonl(B), htonl(C), htonl(D));
    return res;*/
}
