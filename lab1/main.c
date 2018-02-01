/* main.c
 *
 * Assembler program.
 *
 * By Cooper Raterink and Garret Blevins
 */

#include <stdio.h>
#include <string.h>
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
 char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
	                   *lArg2, *lArg3, *lArg4;
  iline_t parsedInstr;
  int lRet;
  do {
	lRet = readAndParse( inFile, lLine, &lLabel,
		                                &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
    strncpy(parsedInstr.label, lLabel, MAX_LINE_LENGTH + 1);
    strncpy(parsedInstr.op, lOpcode, MAX_LINE_LENGTH + 1);
    strncpy(parsedInstr.arg1, lArg1, MAX_LINE_LENGTH + 1);
    strncpy(parsedInstr.arg2, lArg2, MAX_LINE_LENGTH + 1);
    strncpy(parsedInstr.arg3, lArg3, MAX_LINE_LENGTH + 1);
    strncpy(parsedInstr.arg4, lArg4, MAX_LINE_LENGTH + 1);
	if(lRet != DONE && lRet != EMPTY_LINE){ func(parsedInstr); }
  } while(lRet != DONE);
}

void printOut(FILE* outFile, int* instrs, int numInstrs){
  while(numInstrs-- > 0) fprintf(outFile, "0x%.4X\n", *(instrs++));
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
  /*printOut(outfile, (int *)assembledInstrs, currInstr);*/ /* Removed because currIn                                                                     str is undefined. */

  fclose(infile);
  fclose(outfile);
}

