#include "convert.h"
#include "parse.h"
#include "error.h"
#include <stdlib.h>

#define HIGH_REG_SHIFT 9
#define MID_REG_SHIFT  6
#define LOW_REG_SHIFT  0

/* Check if a register argument is valid and if so convert it to instruction bits.
 * Input: register argument string "rx", and the amount to shift the bits for instruction. 
 * Output: error code or register instruction bits.
 */
int regConvert(char* arg, int leftShift){
  if(isRegisterStr(arg) != 1) return OTHER;
  arg++;
  int reg = atoi(arg);
  if(reg <= 7 && reg >= 0)    return reg << leftShift;
  else                        return OTHER;
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
 * Input: value and number of bits allowed to use for immediate value.
 * Output: true or false,
 */
int inBounds(int num, int numBits){
  // (num < (2 ** (numBits - 1))) && (num > -(2 ** (numBits - 1)));
  if((num < (2 ^ (numBits - 1))) && (num > -(2 ^ (numBits - 1)))) return 1;
  else                                                            return 0;
}

int immN(char* arg, int numBits, int immType){
  if(inBounds(toNum(arg), numBits)){
    return toNum(arg);
  }
}

