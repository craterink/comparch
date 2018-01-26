/* opStrToNum.c
 *
 * Converts an opcode string to the opcode number . 
 */

/* Library of strings to use for comparison. */
char addStr[]  = "add";
char andStr[]  = "and";
char brStr[]   = "br";
char jmpStr[]  = "jmp";
char jsrStr[]  = "jsr";
char jsrrStr[] = "jsrr";
char ldbStr[]  = "ldb";
char ldwStr[]  = "ldw";
char leaStr[]  = "lea";
char rtiStr[]  = "rti";
char shfStr[]  = "shf";
char stbStr[]  = "stb";
char stwStr[]  = "stw";
char trapStr[] = "trap";
char xorStr[]  = "xor";
char fillStr[] = ".fill";
char origStr[] = ".orig";
char endStr[]  = ".end";

/* The conversion function. 
 * Input: opcode string.
 * Output: opcode number. 
 */
int opStrToNum(char* opStr){
  int opNum;

       if(strcmp(opStr, addStr)  == 0) opNum = 1;
  else if(strcmp(opStr, andStr)  == 0) opNum = 5;
  else if(strcmp(opStr, brStr)   == 0) opNum = 0;
  else if(strcmp(opStr, jmpStr)  == 0) opNum = 12;
  else if(strcmp(opStr, jsrStr)  == 0) opNum = 4;
  else if(strcmp(opStr, jsrrStr) == 0) opNum = 4;
  else if(strcmp(opStr, ldbStr)  == 0) opNum = 2;
  else if(strcmp(opStr, ldwStr)  == 0) opNum = 6;
  else if(strcmp(opStr, leaStr)  == 0) opNum = 14;
  else if(strcmp(opStr, rtiStr)  == 0) opNum = 8;
  else if(strcmp(opStr, shfStr)  == 0) opNum = 13;
  else if(strcmp(opStr, stbStr)  == 0) opNum = 3;
  else if(strcmp(opStr, stwStr)  == 0) opNum = 7;
  else if(strcmp(opStr, trapStr) == 0) opNum = 15;
  else if(strcmp(opStr, xorStr)  == 0) opNum = 9;
  else if(strcmp(opStr, fillStr) == 0) opNum = 16;
  else if(strcmp(opStr, origStr) == 0) opNum = 17;
  else if(strcmp(opStr, endStr)  == 0) opNum = 18;

  return opNum; 
}

