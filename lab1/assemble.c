#include "assemble.h"
#include "convert.h"
#include "error.h"
#include "opStrToNum.h"

#define NUM_INSTR_VARIATIONS 3
#define MAX_INSTR_STR_LEN 5

#define REGARG		 	0
#define NOARG			0
#define AMOUNT4 		4
#define IMM5 			5
#define OFFSET6 		6
#define TRAPVEC8 		8
#define PCOFFS9 		9
#define PCOFFS11 		11

typedef int (*arg_parse_fn_t)(char *);
typedef int (*dupl_instr_fn_t)(iline_t parsedInstr);

typedef struct instr {
	char op[MAX_INSTR_STR_LEN + 1];
	
	// used only for instructions with variants (e.g. ADD, AND)
	dupl_instr_fn_t isInstOfThisInstr;

	// used to convert arg to value to be used in instr summation
	arg_parse_fn_t arg1Convert;
	int immNumBitsArg1;
	int imm1Type;
	arg_parse_fn_t arg2Convert;
	int immNumBitsArg2;
	int imm2Type;
	arg_parse_fn_t arg3Convert;
	int immNumBitsArg3;
	int imm3Type;
} instr_t;

int isInstrWithImm(iline_t parsedInstr) {
	return isRegisterStr(parsedInstr.arg3);
}
int isInstrNoImm(iline_t parsedInstr) {
	return !isInstrWithImm(parsedInstr);
}

instr_t instrs[NUM_INSTR_VARIATIONS] = {
	{ // ADD 3reg
		"ADD",
		isInstrNoImm,
		regHigh, REGARG, NOCONST,
		regMed, REGARG, NOCONST,
		regLow, REGARG, NOCONST
	},
	{ // ADD w/ imm5
		"ADD",
		isInstrWithImm,
		regHigh, REGARG, NOCONST,
		regMed, REGARG, NOCONST,
		NULL, IMM5, NEG_ALLOWED
	},
	{ // JMP
		"JMP",
		NULL,
		regMed, REGARG, NOCONST,
		NULL, NOARG, NOCONST,
		NULL, NOARG, NOCONST
	}
};

int isOpcodeMatch(instr_t instr, char* op) {
	return !strcmp(instr.op, op);
}

int isMatchingInstr(instr_t instr, iline_t parsedInstr) {
	if(isOpcodeMatch(instr, parsedInstr.op)) {
		if(instr.isInstOfThisInstr != NULL) {
			return instr.isInstOfThisInstr(parsedInstr);
		} else {
			return 1;
		}
	} 
	else {
		return 0;
	}
}

instr_t* matchInstr(iline_t parsedInstr) {
	int i;
	for(i=0;i<NUM_INSTR_VARIATIONS;i++) {
		instr_t * instr = &instrs[i];
		if(isMatchingInstr(*instr, parsedInstr)) {
			return instr;
		}
	}
	return NULL;
}

int argVal(arg_parse_fn_t convert, int immBits, int immType, char * arg) {
	int val;
	if(convert) {
		return convert(arg);
	} else {
		if(immBits == NOARG) {
			// do nothing
		} else if (immBits == REGARG) {
			error(OTHER);
		} else {
			return immN(arg, immBits, immType);
		}	
	}
}

int performAssemble(instr_t * instr, iline_t parsedInstr) {
	int instrVal = opStrToNum(parsedInstr.op, parsedInstr.arg3);
	instrVal += argVal(instr->arg1Convert, 
			instr->immNumBitsArg1, instr->imm1Type, parsedInstr.arg1);
	instrVal += argVal(instr->arg2Convert,
			instr->immNumBitsArg2, instr->imm2Type, parsedInstr.arg2);
	instrVal += argVal(instr->arg3Convert,
			instr->immNumBitsArg3, instr->imm3Type, parsedInstr.arg3);
}

void assembleInstr(iline_t parsedInstr){

	// for each line, go through instrs[] and find matching instr, 
	// then convert using summing procedure
	instr_t* assocInstr = matchInstr(parsedInstr);
	if(!assocInstr) error(INVALID_OPCODES);
	int instrVal = performAssemble(assocInstr, parsedInstr);
	
}