/* main.c
 *
 * Assembler program.
 *
 * By Cooper Raterink and Garret Blevins
 */
#include <stdio.h>
#include "error.h"
#include "parse.h"
#include "num.h"
#include "symtable.h"
#include "opStrToNum.h"
#include "assemble.h"
#include "error.h"

FILE* infile  = NULL;
FILE* outfile = NULL;

void doForEachLine(FILE* inFile, void (*func)(iline_t)){
	char lLine[MAX_LINE_LENGTH + 1];
	iline_t parsedInstr;
	int lRet;
	do {
		lRet = readAndParse(inFile, lLine, (char **)&parsedInstr.label,
				(char **)&parsedInstr.op, (char **)&parsedInstr.arg1, (char **)&parsedInstr.arg2,
				(char **)&parsedInstr.arg3, (char **)&parsedInstr.arg4);
		if(lRet != DONE && lRet != EMPTY_LINE){
			func(parsedInstr);
		}
	} while(lRet != DONE);
}

void printOut(FILE* outFile, int* instrs, int numInstrs){
	while(numInstrs-- > 0)
		fprintf( outFile, "0x%.4X\n", *(instrs++);
}

void parseArgs(int argc, char* argv[]){
	if (argc != 3){
		printf("invalid args. example: './prog <in.asm> <out.obj>'\n");
		error(OTHER);
	}

	char *prgName = NULL;
	char *iFileName = NULL;
	char *oFileName = NULL;

	prgName = argv[0];
	iFileName = argv[1];
	oFileName = argv[2];

	infile  = fopen(iFileName, "r");
	outfile = fopen(oFileName, "w");

	if (!infile){
		printf("Error: Cannot open file %s\n", argv[1]);
		error(OTHER);
	}
	if (!outfile){
		printf("Error: Cannot open file %s\n", argv[2]);
		error(OTHER);
	}
}

int main(int argc, char* argv[]){
	parseArgs(argc, argv);

	/* Pass 1 */
	initSymTableBldr(); /* Build symbol table and count the number of lines. */
	doForEachLine(infile, buildSymTable);

	/* Pass 2 */
	rewind(infile); /* Rewind the cursor to the beginning of the input file.*/
	doForEachLine(infile, assembleInstr);
	printOut(outfile, (int *)assembledInstrs, currInstr);

	fclose(infile);
	fclose(outfile);
}

