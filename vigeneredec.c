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

double calc_max_pr(int *stream, int size) {
    int qCnt = 0, i = 0, j = 0, procPos = 0, cnt = 0;
    int *chSet = (int*) calloc(size, sizeof(int));
    int *stat = (int*) calloc(size, sizeof(int));
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
            printf("%c [ %3d ]", stream[i], stream[i]);
            continue;
        }
        cnt++;
        if (procPos > 0) {
            j = 0;
            while (j < procPos && stream[i]!= chSet[j]) {
              j++;
            }
            if (j < procPos) {
                continue;
            }
        }
        qCnt = 0;
        for (j = 0; j < size; j++) {
            if (stream[j] == stream[i]) {
                qCnt++;
            }
        }
        chSet[procPos] = stream[i];
        stat[procPos] = qCnt;
        procPos++;
        printf("%c[%3d]: %d / %d - %5.3f\n", stream[i], stream[i], qCnt, size, pr);
    }
    for (i = 0; i < procPos; i++) {
        pr += (double) stat[i] / cnt * alphabetFrequency[chSet[i]];
        printf("%c[%3d]: %d / %d = %4.5f (%4.5f) - %5.5f\n", chSet[i], chSet[i], stat[i], cnt, (double) stat[i] / cnt, alphabetFrequency[chSet[i]], pr);
    }
    free(stat);
    free(chSet);
    return pr;
}

double calcKeyLengthProb(int *stream, int size) {
    int i = 0, j = 0, k = 0, cnt = 0;

    int *setVal = (int*) calloc(size, sizeof(int));
    int *setCnt = (int*) calloc(size, sizeof(int));

    double pr = 0.0;

    for (i = 0; i < size; i++) {
        if (k > 0) {
            j = 0;
            while (j < k && stream[i] != setVal[j]) {
                j++;
            }
            if (j < k) {
                continue; //zise char olready counted
            }
        }
        setVal[k] = stream[i];
        cnt = 0;
        for (j = 0; j < size; j++) {
            if (stream[i] == stream[j]) {
                cnt++;
            }
        }
        setCnt[k] = cnt;
        k++;
    }
    print_int_array(setVal, k, "%c ", true);
    print_int_array(setCnt, k, "%d ", true);

    for (i = 0; i < k; i++) {
        pr += setCnt[i] * setCnt[i] / (size * size);
    }
    free(setVal);
    free(setCnt);
    return pr;
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

    int l = 0, hex = 0, breakPos = 0;
    double prSet[256], pr;
    //pos = cnt;
    int *stream = (int*) calloc(cnt, sizeof(int));
    //int key[KEY_LENGTH];
    for (l=1; l < 14; l++) {
        for (i = 0; i < 256; i++) {
            prSet[i] = 0.0;
        }
        for (hex = 0; hex < 256; hex++) {
            breakPos = -1;
            for (i = 0; i < cnt; i++) {
                stream[i] = text[i*l] ^ hex;
                if (stream[i] < 32 || stream[i] > 127) {
                    breakPos = i;
                    //printf("key byte %3d do not correct, over from print chars on %d position\n", hex, breakPos);
                    break;
                }
                //printf("%3d ^ %3d = %3d (%c)\n", text[i], hex, stream[i], stream[i]);
            }
            if (breakPos >= 0) {
                continue;
            }
            //printf("is hex looks good(%d): %4d\n", breakPos, hex);
            //print_int_array(stream, pos, "%c ", true);
            pr = calcKeyLengthProb(stream, cnt);
            prSet[hex] = pr;
            printf("%3d: %5.3f\n\n", hex, pr);
        }
        //double max = 0.0;
        //for (i = 0; i < 256; i++) {
        //    if (prSet[i] > max && prSet[i] < 0.068) {
        //        max = prSet[i];
        //        hex = i;
        //    }
        //}
        //key[l] = hex;
        print_double_array(prSet, 256, "%6.5f ", false);
        //printf("%d - %3d[%02X]: %6.5f\n", l, hex, hex, max);
    }
    free(stream);
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
    return 0;
}
//My answer was accepted and it has SHA1:
//532a23b63b23b0d2a99e16f1e68e88eafe4148cb
