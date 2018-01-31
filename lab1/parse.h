#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 255
#define MAX_ADDR (1 << 16) - 1

enum {
	DONE, OK, EMPTY_LINE
};

typedef struct iline {
	char label[MAX_LINE_LENGTH + 1];
	char op[MAX_LINE_LENGTH + 1];
	char arg1[MAX_LINE_LENGTH + 1];
	char arg2[MAX_LINE_LENGTH + 1];
	char arg3[MAX_LINE_LENGTH + 1];
	char arg4[MAX_LINE_LENGTH + 1];
} iline_t;

int readAndParse(FILE* pInfile, char* pLine, char** pLabel, char** pOpcode, 
                 char** pArg1, char** pArg2, char** pArg3, char** pArg4);

int isOpcode(char* str);

int isValidLabel(char* str);

int isiValidAddr(int addr);

int isRegisterStr(char* str);

#endif

