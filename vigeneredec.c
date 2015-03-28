#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define KEY_LENGTH 7

int hex_to_int(char c) {
    if(c >=97)
        c=c-32;
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

void print_double_array(double *array, int size, char *format, bool all) {
    int i = 0;
    for (i = 0; i < size; i++) {
        if (!all && array[i] <= 0) {
            continue;
        }
        printf(format, array[i]);
    }
    printf("\n");
}

int countElementInArray(int *array, int size, int element) {
    int cnt = 0, i = 0;
    for (i = 0; i < size; i++) {
        if (element == array[i]) {
            cnt++;
        }
    }
    return cnt;
}

int findElementInArray(int *array, int size, int element) {
    int j = 0;
    while (j < size && element != array[j]) {
      j++;
    }
    return j;
}

double calcKeyBytes(int *stream, int size) {
    int i = 0, k = 0, cnt = 0;
    int *setVal = (int*) calloc(size, sizeof(int));
    int *setCnt = (int*) calloc(size, sizeof(int));
    double alphabetFrequency[256] = {};
    alphabetFrequency[97]  = 0.08167;
    alphabetFrequency[98]  = 0.01492;
    alphabetFrequency[99]  = 0.02782;
    alphabetFrequency[100] = 0.04253;
    alphabetFrequency[101] = 0.12702;
    alphabetFrequency[102] = 0.02228;
    alphabetFrequency[103] = 0.02015;
    alphabetFrequency[104] = 0.06094;
    alphabetFrequency[105] = 0.06966;
    alphabetFrequency[106] = 0.00153;
    alphabetFrequency[107] = 0.00772;
    alphabetFrequency[108] = 0.04025;
    alphabetFrequency[109] = 0.02406;
    alphabetFrequency[110] = 0.06749;
    alphabetFrequency[111] = 0.07507;
    alphabetFrequency[112] = 0.01929;
    alphabetFrequency[113] = 0.00095;
    alphabetFrequency[114] = 0.05987;
    alphabetFrequency[115] = 0.06327;
    alphabetFrequency[116] = 0.09056;
    alphabetFrequency[117] = 0.02758;
    alphabetFrequency[118] = 0.00978;
    alphabetFrequency[119] = 0.02360;
    alphabetFrequency[120] = 0.00150;
    alphabetFrequency[121] = 0.01974;
    alphabetFrequency[122] = 0.00074;
    double pr = 0.0;
    for (i = 0; i < size; i++) {
        if (stream[i]< 97 || stream[i]> 122) {
            continue;
        }
        cnt++;
        if (k > 0 && findElementInArray(setVal, k, stream[i]) < k) {
            continue;
        }
        setVal[k] = stream[i];
        setCnt[k] = countElementInArray(stream, size, stream[i]);
        k++;
    }
    if (k == 0) {
        return 0.0;
    }
    print_int_array(stream, size, "%3d ", false);
    print_int_array(setCnt, size, "%3d ", false);
    print_int_array(setVal, size, "%3c ", false);
    for (i = 0; i < k; i++) {
        pr += (double) setCnt[i] / cnt * alphabetFrequency[setVal[i]];
        printf("%c[%3d]: %d / %d = %4.5f (%4.5f) - %5.5f\n", setVal[i], setVal[i], setCnt[i], cnt, (double) setCnt[i] / cnt, alphabetFrequency[setVal[i]], pr);
    }
    free(setVal);
    free(setCnt);
    return pr;
}

double calcKeyLengthProb(int *stream, int size) {
    int i = 0, k = 0;

    int *setVal = (int*) calloc(size, sizeof(int));
    int *setCnt = (int*) calloc(size, sizeof(int));

    double pr = 0.0;

    for (i = 0; i < size; i++) {
        if (k > 0 && findElementInArray(setVal, k, stream[i]) < k) {
            continue; //zise char olready counted
        }
        setVal[k] = stream[i];
        setCnt[k] = countElementInArray(stream, size, stream[i]);
        k++;
    }

    for (i = 0; i < k; i++) {
        pr += (double) setCnt[i] * setCnt[i] / (size * size);
    }
    free(setVal);
    free(setCnt);
    return pr;
}

int defKeyLength(int *text, int cnt) {
    int i = 0, j = 0, l = 0, hex = 0, k = 0;
    double pr;
    double *prSet = (double*) calloc(14, sizeof(double));
    int *stream = (int*) calloc(cnt, sizeof(int));
    for (l=1; l < 14; l++) {
        for (hex = 0; hex < 256; hex++) {
            k = -1;
            for (i = 0; i < cnt / l; i++) {
                stream[i] = text[i*l] ^ hex;
                if (stream[i] < 32 || stream[i] > 127) {
                    k = i;
                    break;
                }
            }
            if (k >= 0) {
                continue;
            }
            //print_int_array(stream, cnt / l, "%3d ", false);
            pr = calcKeyLengthProb(stream, cnt / l);
            prSet[l] += pr;
        }
    }
    pr = 0.0;
    for (i = 1; i < 14; i++) {
        if (prSet[i] > pr) {
            pr = prSet[i];
            l = i;
        }
    }
    free(stream);
    free(prSet);
    return l;
}

void findKey(int *text, int cnt, int *key, int size) {
    int i = 0, j = 0, l = 0, hex = 0, k = 0;
    double pr;
    double *prSet = (double*) calloc(256, sizeof(double));
    int *stream = (int*) calloc(cnt / size, sizeof(int));

    for (l = 0; l < size; l++) {
        for (hex = 0; hex < 256; hex++) {
            k = -1;
            for (i = 0; i < cnt / size; i++) {
                stream[i] = text[l * i + l] ^ hex;
                if (stream[i] < 32 || stream[i] > 127) {
                    k = i;
                    break;
                }
            }
            if (k >= 0) {
                continue;
            }
            print_int_array(stream, cnt / size, "%3d ", false);
            prSet[hex] = calcKeyBytes(stream, cnt / size);
        }
        pr = 0.0;
        k = 0;
        for (hex = 0; hex < 256; hex++) {
            if (prSet[hex] > pr && prSet[hex] < 0.0665) {
                pr = prSet[hex];
                k = hex;
            }
        }
        key[l] = k;
    }
    print_int_array(key, l, "%02X ", true);

    free(stream);
    free(prSet);
}

int main(int argc, char *argv[])
{
    FILE *fp;

    fp = fopen(argv[1],"r"); // read mode

    if( fp == NULL )
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *bytes = (char*) calloc(pos, sizeof(char));
    fread(bytes, pos, 1, fp);
    fclose(fp);

    int cnt = pos / 2, i = 0;
    printf("file %s content: %ld, chars: %d\n", argv[1], pos, cnt);
    int *text = (int*) calloc(cnt, sizeof(int));

    for (i = 0; i < cnt; i++) {
        text[i] = hex_to_ascii(bytes[2*i], bytes[2*i+1]);
    }
    free(bytes);
    print_int_array(text, cnt, "%3d ", true);
    print_int_array(text, cnt, "%02X ", true);

    int l = defKeyLength(text, cnt);
    int *key = (int*) calloc(l, sizeof(int));
    findKey(text,cnt,key, l);
    printf("\nUsed key:\n");
    print_int_array(key, l, "%02X ", true);
/*
    printf("\nUsed key:\n");
    print_int_array(key, KEY_LENGTH, "%02X ", true);
    printf("\nDecrypted text:\n");
    char ch;
    for (i=0; i < cnt; i++) {
        ch = text[i] ^ key[i % KEY_LENGTH];
        printf("%c", ch);
    }
    */
    free(text);
    free(key);
    return 0;
}
//My answer was accepted and it has SHA1:
//532a23b63b23b0d2a99e16f1e68e88eafe4148cb
