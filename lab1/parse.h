#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 255

enum {
	DONE, OK, EMPTY_LINE
};

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
        ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
        );


int isOpcode(char * str);
