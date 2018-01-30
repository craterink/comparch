#include "convert.h"
#include "parse.h"

#define HIGH_REG_SHIFT 9
#define MID_REG_SHIFT 6
#define LOW_REG_SHIFT 0


int regConvert(char * arg, int leftShift) {
	return 0;
}

int regHigh(char * arg) {
	return regConvert(arg, HIGH_REG_SHIFT);
}

int regMed(char * arg) {
	return regConvert(arg, MID_REG_SHIFT);
}

int regLow(char * arg) {
	return regConvert(arg, LOW_REG_SHIFT);
}

int inBounds(int num, int numBits) {
	return 0;//(num < (2 ** (numBits - 1))) && (num > -(2 ** (numBits - 1)));
}

int immN(char * arg, int numBits, int immType) {
	if(inBounds(toNum(arg), numBits)) {
		return toNum(arg);
	}
}
