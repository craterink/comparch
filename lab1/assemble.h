#ifndef _ASSEMBLE_H_
#define _ASSEMBLE_H_

#include "parse.h"
#include "symtable.h"

extern int assembledInstr[MAX_NUM_INSTRS]; 

void assembleInstr(iline_t parsedInstr);

#endif
