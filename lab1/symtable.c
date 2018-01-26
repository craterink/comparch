#include "symtable.h"
#include "num.h"
#include "parse.h"
#include "error.h"

#define INIT_ADDR -1

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
	int tableIndex;
} sym_table_bldr_t;
sym_table_bldr_t symTableBldr;

void initSymTableBldr(void) {
	symTableBldr.startAddr = INIT_ADDR;
	symTableBldr.finishAddr = INIT_ADDR;
	symTableBldr.currAddr = INIT_ADDR;
}

void addLabelToSymTable(char * label) {
	if(symTableBldr.tableIndex >= MAX_NUM_LABELS) error(OTHER);

	sym_table_entry_t labelEntry;
	strncpy((char *)labelEntry.label, label, LABEL_MAX_STR_LEN);
	labelEntry.addr = symTableBldr.currAddr;
	symTableBldr.symTable[symTableBldr.tableIndex++] = labelEntry;
}

void buildSymTable(char * label, char * opcode, char * arg1,
		char * arg2, char * arg3, char * arg4) {
	if(strlen(label)) {
		// check label validity, enforcing no label on .orig or .end
		if(!isValidLabel(label)
				|| !strcmp(opcode, ORIG)
				|| !strcmp(opcode, END))
			error(OTHER);

		addLabelToSymTable(label);
	}

	// handle .orig
	if (!strcmp(opcode, ORIG)) {
		if(symTableBldr.startAddr != INIT_ADDR // enforce .orig is first opcode 
				|| symTableBldr.currAddr != INIT_ADDR
				|| symTableBldr.finishAddr != INIT_ADDR) error(OTHER);

		symTableBldr.startAddr = toNum(arg1);
		if(!isValidAddr(symTableBldr.startAddr)) error(OTHER);
		symTableBldr.currAddr = symTableBldr.startAddr;
	}

	// handle .end
	if (!strcmp(opcode, END)) {
		symTableBldr.finishAddr = symTableBldr.currAddr - 1;
	}

	symTableBldr.currAddr++;
}
