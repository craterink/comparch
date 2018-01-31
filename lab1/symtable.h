#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include <stdlib.h>
#include "parse.h"

/* psuedo-ops */
#define ORIG ".orig"
#define FILL ".fill"
#define END  ".end"

/* as given by lab doc */
#define MAX_NUM_INSTRS 255
#define LABEL_MAX_STR_LEN 21
#define MAX_NUM_LABELS MAX_NUM_INSTRS

void initSymTableBldr(void);

void buildSymTable(iline_t parsedInstr);

/**
 * returns LABEL's addr if its in the table,
 *  or -1 if not.
 */
int symbolAddr(char* label);

int labelToOffset(char* label, int fromAddr);

int getNumInstrs(void);

#endif

