#include "convert.h"
#include "parse.h"
#include "error.h"
#include "assemble.h"
#include <stdlib.h>
#include <math.h>

#define HIGH_REG_SHIFT 9
#define MID_REG_SHIFT  6
#define LOW_REG_SHIFT  0

#define HIGHEST_REG 7

/* Check if a register argument is valid and if so convert it to instruction bits.
 * Input: register argument string "rx", and the amount to shift the bits for instruction. 
 * Output: error code or register instruction bits.
 */
int regConvert(char* arg, int leftShift){
	if(isRegisterStr(arg) != 1) return OTHER;
	arg++;
	int reg = atoi(arg);
	if(reg <= HIGHEST_REG && reg >= 0)    return reg << leftShift;
	else                                  return OTHER;
}

int regHigh(char* arg){
	return regConvert(arg, HIGH_REG_SHIFT);
}

int regMed(char* arg){
	return regConvert(arg, MID_REG_SHIFT);
}

int regLow(char* arg){
	return regConvert(arg, LOW_REG_SHIFT);
}

/* Check if an immediate value argument is valid.
 * Input: value, number of bits allowed to use for immediate value, and if the immediate
 *        value can be negative.
 * Output: true or false.
 */
int inBounds(int num, int numBits, int isNegAllowed){
	if(isNegAllowed == NEG_ALLOWED){
		return (num <=  (int)pow(2, numBits - 1) - 1) 
			&& (num >= -(int)pow(2, numBits - 1));
	}
	else {
		return (num <=  (int)pow(2, numBits) - 1) 
			&& (num >= 0);
	}
}

int immN(char* arg, int numBits, int immType, int currAddr){
	int isNegAllowed = (immType == NEG_ALLOWED || immType == BYTE_OFFSET);
	int imm = isImm(arg) ? toNum(arg) : labelToOffset(arg, currAddr);
	if(isSymbolInTable(arg) && immType == BYTE_OFFSET) imm = imm*ADDRESSABILITY;
	if(inBounds(imm, numBits, isNegAllowed)){
		/* Mask offsets and immediate values. */
		if(numBits == BOFFSET6 && immType == BYTE_OFFSET) 
			return imm & 0x3F;
		else if(numBits ==  OFFSET6) return  imm                   & 0x3F;
		else if(numBits ==  PCOFFS9) return  imm                   & 0x1FF;
		else if(numBits == PCOFFS11) return  imm                   & 0x7FF;
		else if(numBits ==     IMM5) return  imm                   & 0x1F;
		else						 return	 imm;
	}    
	else {
		if(isImm(arg)) error(INVALID_CONSTANTS);
		else error(OTHER);
	}
}

