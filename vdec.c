#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define KEY_LENGTH 7 // Can be anything from 1 to 13

int hex_to_int(char c) {
    int first = c / 16 - 3;
    int second = c % 16;
    int result = first*10 + second;
    if(result > 9) result--;
    return result;
}

int hex_to_ascii(char c, char d) {
    int high = hex_to_int(c) * 16;
    int low = hex_to_int(d);
    return high+low;
}

void print_int_array(int *array, int size, char *format, bool all) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if (!all && array[i] <= 0) {
            continue;
        }
        printf(format, array[i]);
    }
    printf("\n");
}

int main(){
    unsigned char ch;
    FILE *fpIn, *fpOut;
    int i;
    unsigned char key[KEY_LENGTH] = {0xAB,0x56,0xC2,0xB2,0x53,0x9E,0x3E};
    /* of course, I did not use the all-0s key to encrypt */

    fpIn = fopen("ctext.txt", "r");
    fpOut = fopen("dtext.txt", "w");

    fseek(fpIn, 0, SEEK_END);
    long pos = ftell(fpIn);
    fseek(fpIn, 0, SEEK_SET);

    char *bytes = (char*) calloc(pos, sizeof(char));
    fread(bytes, pos, 1, fpIn);
    fclose(fpIn);

    int cnt = pos / 2;

    int *text = (int*) calloc(cnt, sizeof(int));

    printf("input hex:\n");
    for (i = 0; i < cnt; i++) {
        text[i] = hex_to_ascii(bytes[2*i], bytes[2*i+1]);
        printf("%c%c", bytes[2*i], bytes[2*i + 1]);
    }
    free(bytes);

    printf("\ntransferm input hex:\n");
    print_int_array(text, cnt, "%02X", true);
    printf("input charCode:\n");
    print_int_array(text, cnt, "%d ", true);

    printf("decode chars:\n");
    for (i = 0; i < cnt; i++) {
        ch = text[i] ^ key[i % KEY_LENGTH];
        fprintf(fpOut, "%c", ch);
        printf("%c", ch);
    }
    free(text);
    fclose(fpIn);
    fclose(fpOut);
    return 0;
}
