#ifndef _ASSEMBLE_H_
#define _ASSEMBLE_H_

#include "parse.h"
#include "symtable.h"

#define NUM_INSTR_VARIATIONS 31
#define MAX_INSTR_STR_LEN    5
#define REGARG		     0
#define NOARG	             0
#define AMOUNT4 	     4
#define IMM5 		     5
#define OFFSET6 	     6
#define BOFFSET6	     7
#define TRAPVEC8 	     8
#define PCOFFS9 	     9
#define PCOFFS11 	     11
#define AMOUNT16             16

extern int assembledInstr[MAX_NUM_INSTRS]; 

void assembleInstr(iline_t parsedInstr);

#endif

