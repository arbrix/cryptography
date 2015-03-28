#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int getVal(char c) {
   int rtVal = 0;
   if (c >= '0' && c <= '9') {
       rtVal = c - '0';
   } else {
       rtVal = c - 'A' + 10;
   }
   return rtVal;
}

int main(int argc, char *argv[])
{
   /*
   char ch, buf, * chSet;
   int statCnt[256], fSize, cnt;
   */
   FILE *fp;

   fp = fopen(argv[1],"r"); // read mode

   if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   char c;
   int i = 0;
   while((c = fgetc(fp))  != EOF) {
      int val = getVal(c) * 16 + getVal(fgetc(fp));
      printf("%3d - %c [%d]\n", i, val, val);
      i++;
   }
   fclose(fp);
   return 0;
   /*
   while(fgetc(fp) != EOF ) {
      cnt++;
   }
   fSize = cnt / 2;
   printf("%s file size is :%i\n", argv[1], cnt);

   int i = 0, pos = 0, minShift = 0, maxShift = 0, shiftCnt = 0;
   cnt = 0;
   while( ( ch = fgetc(fp) ) != EOF ) {
      i += 1;
      if (i == 2) {
        pos = hex_to_ascii(buf, ch);
        statCnt[pos]++;
        minShift = pos - 127;
        maxShift = pos - 32;
        shiftCnt = maxShift - minShift;
        printf("%4u: %03u - min: %3i; max: %3i; count: %3i\n\r", cnt, pos, minShift, maxShift,shiftCnt);
        i = 0;
          cnt++;
      }
      buf = ch;
   }

   fclose(fp);
   return 0;
   */
}
