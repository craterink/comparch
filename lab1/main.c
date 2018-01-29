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

FILE* infile  = NULL;
FILE* outfile = NULL;
int numLines  = 0;

void assess(char* label, char* opcode, char* arg1, char* arg2, char* arg3, 
            char* arg4){
  numLines++;
}

void assembleInstr(char* label, char* opStr, char* arg1Str, char* arg2Str,
                   char* arg3Str, char* arg4Str){
  int opNum = opStrToNum(opStr);
  
  /* Handle registers, immediates, and/or offsets. */
       if((opNum == 1 && sr2) || (opNum == 5 && sr2) || (opNum == 9 && sr2)){

  }
  else if((opNum == 1 && imm5) || (opNum == 5 && imm5) || (opNum == 9 && imm5)){

  }
  else if(opNum == 0){
    
  }
  else if(opNum == 4){
    
  }
  else if(){
    
  }
}

void doForEachLine(FILE* inFile, void (*func)(char*, char*, char*, char*, 
                   char*, char*)){
  char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
  int lRet;
  do {
    lRet = readAndParse(inFile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
    if(lRet != DONE && lRet != EMPTY_LINE){
      func(lLabel, lOpcode, lArg1, lArg2, lArg3, lArg4);
    }
  } while(lRet != DONE);
}

void printOut(FILE* outFile, int* instrs, int numInstrs){
  while(numInstrs-- > 0) {
    fprintf( outFile, "0x%.4X\n", *instrs);
    instrs++;
  }
}

void parseArgs(int argc, char* argv[]){
  if (argc != 3){
    printf("invalid args. example: './prog <in.asm> <out.obj>'\n");
    exit(4);
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
    exit(4);
  }
  if (!outfile){
    printf("Error: Cannot open file %s\n", argv[2]);
    exit(4);
  }
}

int main(int argc, char* argv[]){
  parseArgs(argc, argv);

  /* Pass 1 */
  initSymTableBldr(); /* Build symbol table and count the number of lines. */
  doForEachLine(infile, buildSymTable);
  int* instrs = malloc(sizeof(int) * numLines);

  /* Pass 2 */
  rewind(infile); /* Rewind the cursor to the beginning of the input file. */
  doForEachLine(infile, assembleInstr);
  printOut(outfile, instrs, numLines);

  fclose(infile);
  fclose(outfile);
}

