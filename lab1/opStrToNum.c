/* opStrToNum.c
 *
 * Converts an opcode string to the opcode number . 
 */

/* Library of strings to use for comparison. */
char addStr[]  = "add";
char andStr[]  = "and";
char brStr[]   = "br";
char brnStr[]  = "brn";
char brzStr[]  = "brz";
char brpStr[]  = "brp";
char brzpStr[] = "brzp";
char brnzStr[] = "brnz";
char brnpStr[] = "brnp";
char brnzpStr[]= "brnzp";
char jmpStr[]  = "jmp";
char jsrStr[]  = "jsr";
char jsrrStr[] = "jsrr";
char ldbStr[]  = "ldb";
char ldwStr[]  = "ldw";
char leaStr[]  = "lea";
char notStr[]  = "not";
char retStr[]  = "ret";
char rtiStr[]  = "rti";
char lshfStr[] = "lshf";
char rshflStr[]= "rshfl";
char rshfaStr[]= "rshfa";
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
int opStrToNum(char* opStr, char* arg3){
  int opNum;

  if(strcmp(opStr, addStr)  == 0){
    if(arg3[0] == 'r') opNum = 4096;
    else               opNum = 4128;  
  }
  else if(strcmp(opStr, andStr)  == 0){
    if(arg3[0] == 'r') opNum = 20480;
    else               opNum = 20512;
  }
  else if(strcmp(opStr, brStr)   == 0 || strcmp(opStr, brnzpStr) == 0) opNum = 0;
  else if(strcmp(opStr, brnStr)  == 0) opNum = 2048;
  else if(strcmp(opStr, brzStr)  == 0) opNum = 1024;
  else if(strcmp(opStr, brpStr)  == 0) opNum = 512;
  else if(strcmp(opStr, brzpStr) == 0) opNum = 1536;
  else if(strcmp(opStr, brnzStr) == 0) opNum = 3072;
  else if(strcmp(opStr, brnpStr) == 0) opNum = 2560; 
  else if(strcmp(opStr, jmpStr)  == 0) opNum = 49152;
  else if(strcmp(opStr, jsrStr)  == 0) opNum = 18432;
  else if(strcmp(opStr, jsrrStr) == 0) opNum = 16384;
  else if(strcmp(opStr, ldbStr)  == 0) opNum = 8192;
  else if(strcmp(opStr, ldwStr)  == 0) opNum = 24576;
  else if(strcmp(opStr, leaStr)  == 0) opNum = 57344;
  else if(strcmp(opStr, notStr)  == 0) opNum = 36927;
  else if(strcmp(opStr, retStr)  == 0) opNum = 49600;
  else if(strcmp(opStr, rtiStr)  == 0) opNum = 32768;
  else if(strcmp(opStr, lshfStr) == 0) opNum = 53248;
  else if(strcmp(opStr, rshflStr)== 0) opNum = 53264;
  else if(strcmp(opStr, rshfaStr)== 0) opNum = 53296;
  else if(strcmp(opStr, stbStr)  == 0) opNum = 12288;
  else if(strcmp(opStr, stwStr)  == 0) opNum = 28672;
  else if(strcmp(opStr, trapStr) == 0) opNum = 61440;
  else if(strcmp(opStr, xorStr)  == 0){
    if(arg3[0] == 'r') opNum = 36864;
    else               opNum = 36896;
  }
  else if(strcmp(opStr, fillStr) == 0) opNum = 16;
  else if(strcmp(opStr, origStr) == 0) opNum = 17;
  else if(strcmp(opStr, endStr)  == 0) opNum = 18;

  return opNum; 
}

