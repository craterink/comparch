#include "symtable.h"
#include "num.h"
#include "parse.h"
#include "error.h"

#define INIT_ADDR      -1

typedef char label_t[LABEL_MAX_STR_LEN];
typedef struct sym_table_entry {
  int addr;
  label_t label;
} sym_table_entry_t;

typedef struct sym_table_bldr {
  int startAddr;
  int finishAddr;
  int currAddr;	
  sym_table_entry_t symTable[MAX_NUM_LABELS];
  int numSymbols;
  int numInstrs;
} sym_table_bldr_t;
sym_table_bldr_t symTableBldr;

void initSymTableBldr(void){
  symTableBldr.startAddr  = INIT_ADDR;
  symTableBldr.finishAddr = INIT_ADDR;
  symTableBldr.currAddr   = INIT_ADDR;
  symTableBldr.numSymbols = 0;
}

int isSymbolInTable(char* label){
  int i;
  for(i = 0; i < symTableBldr.numSymbols; i++){
    if(!strcmp(label, (char *)symTableBldr.symTable[i].label)) return 1;
  }
  return 0;
}

int symbolAddr(char* label){
  int i;
  for(i = 0; i < symTableBldr.numSymbols; i++){
    if(!strcmp(label, (char*)symTableBldr.symTable[i].label)) 
      return symTableBldr.symTable[i].addr;
  }
  return -1;
}

/* returns byte offset / ADDRESSABILTY (instruction offset) */
int labelToOffset(char* label, int fromAddr){
  if(isSymbolInTable(label)){
    int toAddr = symbolAddr(label);
    return (toAddr - fromAddr) / ADDRESSABILITY; 
  } 
  else { error(UNDEFINED_LABELS); }
}

int getNumInstrs(void){
  return symTableBldr.numInstrs;
}

void addLabelToSymTable(char* label){
  if(symTableBldr.numSymbols >= MAX_NUM_LABELS || isSymbolInTable(label)) 
    error(OTHER);
  sym_table_entry_t labelEntry;
  strncpy((char*)labelEntry.label, label, LABEL_MAX_STR_LEN);
  labelEntry.addr = symTableBldr.currAddr;
  symTableBldr.symTable[symTableBldr.numSymbols++] = labelEntry;
}

void buildSymTable(iline_t parsedInstr){
  if(strlen(parsedInstr.label)){
  /* check label validity, enforcing no label on .orig or .end */
  if(!isValidLabel(parsedInstr.label) || !strcmp(parsedInstr.op, ORIG)
				      || !strcmp(parsedInstr.op, END))
    error(OTHER);

  addLabelToSymTable(parsedInstr.label);
  }

  /* handle .end */
  if (!strcmp(parsedInstr.op, END)){
    symTableBldr.finishAddr = symTableBldr.currAddr - ADDRESSABILITY;
    symTableBldr.currAddr = INIT_ADDR;
  }
  /* handle .orig */
  else if (!strcmp(parsedInstr.op, ORIG)){
    if(symTableBldr.startAddr != INIT_ADDR /* enforce .orig is first opcode */
	                              || symTableBldr.currAddr != INIT_ADDR
				      || symTableBldr.finishAddr != INIT_ADDR)
      error(OTHER);

  symTableBldr.startAddr = toNum(parsedInstr.arg1);
  if(!isValidAddr(symTableBldr.startAddr)) error(OTHER);
    symTableBldr.currAddr = symTableBldr.startAddr;
  } 
  else { symTableBldr.currAddr+= ADDRESSABILITY; }

  /* TODO: do some check before increasing? */
  symTableBldr.numInstrs++;
}

