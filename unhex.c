/*  UNHEX.C - Program to translate a hex file from standard input
 *  into an 8-bit binary file on standard output.
 *  Usage: unhex < foo.hex > foo.exe
 *  Christine M. Gianone, CUCCA, October 1986.
 *  Modified Aug 89 to work right with Microsoft C on the PC.
 */
#include <stdlib.h>
#include <stdio.h>			/* Include this for EOF symbol */
#ifdef MSDOS
#include <fcntl.h>			/* For MS-DOS setmode() symbol */
#endif

unsigned char a, b;			/* High and low hex nibbles */
unsigned int c;				/* Character to translate them into */
unsigned char decode();			/* Function to decode them  */

/* Main program reads each hex digit pair and outputs the 8-bit byte. */

int main() {
#ifdef MSDOS
    setmode(fileno(stdout),O_BINARY); /* To avoid DOS text-mode conversions */
#endif
    while ((c = getchar()) != EOF) {	/* Read first hex digit */
	a = c;				/* Convert to character */
        if (a == '\n' || a == '\r') {	/* Ignore line terminators */
            continue;
	}
	if ((c = getchar()) == EOF) {	/* Read second hex digit */
	    fprintf(stderr,"File ends prematurely\n");
	    exit(1);
	}
	b = c;				/* Convert to character */
	putchar((((decode(a) * 16) & 0xF0) + (decode(b) & 0xF)) % 26 + 'A');
    }
    exit(0);				/* Done */
}

unsigned char
decode(x) char x; {		      /* Function to decode a hex character */
    if (x >= '0' && x <= '9')         /* 0-9 is offset by hex 30 */
      return (x - 0x30);
    else if (x >= 'A' && x <= 'F')    /* A-F offset by hex 37 */
      return(x - 0x37);
    else {                            /* Otherwise, an illegal hex digit */
        fprintf(stderr,"\nInput is not in legal hex format\n");
        exit(1);
    }
}
