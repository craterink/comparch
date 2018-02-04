/*
    Name 1: Cooper Raterink 
    Name 2: Garret Blevins
    UTEID 1: cdr2678
    UTEID 2: geb628
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

#define Lowbit(x) ((x) & 0x1)
#define Low8bits(x) ((x) & 0xFF)
#define LeftShiftOneByte(x) ((x) << 8)
#define RightShiftOneByte(x) ((x) >> 8)
#define OpcodeOfInstr(x) (((x) & 0xF000) >> 12)
#define RegHigh(x) (((x) & 0xE00) >> 9)
#define RegMid(x) (((x) & 0x1C0) >> 6)
#define RegLow(x) ((x) & 0x0007)
#define ImmN(n,x) ((x) & ((1 << (n)) - 1))
#define NthBit(n,x) (((x) & (1 << (n))) >> (n))
#define ABit(x) NthBit(5,x)
#define DBit(x) NthBit(4,x)
#define NBit(x) NthBit(11,x)
#define ZBit(x) NthBit(10,x)
#define PBit(x) NthBit(9,x)
#define JSR_ABit(x) NthBit(11,x)

#define SIM_ERROR 4

#define MAX_REG 7
#define PC_REG -1
#define N_CC -2
#define Z_CC -3
#define P_CC -4

#define BYTES_PER_MEM_LOC 2
#define LITTLE_END 0
#define BIG_END 1

#define ADD 0x0001
#define AND 0x0101
#define BR  0x0000
#define JMP 0x1100
#define JSR 0x0100
#define LDB 0x0010
#define LDW 0x0110
#define LEA 0x1110
#define SHF 0x1101
#define STB 0x0011
#define STW 0x0111
#define TRAP 0x1111
#define XOR 0x1001


int loadReg(int regNum) {
	if(regNum >= 0 && regNum <= MAX_REG)
		return NEXT_LATCHES.REGS[regNum];
	else if (regNum == PC_REG)
		return NEXT_LATCHES.PC;
	else if (regNum == N_CC)
		return NEXT_LATCHES.N;
	else if (regNum == Z_CC)
		return NEXT_LATCHES.Z;
	else if (regNum == P_CC)
		return NEXT_LATCHES.P;
	else 
		exit(SIM_ERROR);
}

void storeReg(int regNum, int value) {
	if(regNum >= 0 && regNum <= MAX_REG)
		NEXT_LATCHES.REGS[regNum] = Low16bits(value);
	else if (regNum == PC_REG)
		NEXT_LATCHES.PC = Low16bits(value);
	else if (regNum == N_CC)
		NEXT_LATCHES.N = Lowbit(value);
	else if (regNum == Z_CC)
		NEXT_LATCHES.Z = Lowbit(value);
	else if (regNum == P_CC)
		NEXT_LATCHES.P = Lowbit(value);
	else 
		exit(SIM_ERROR);
}

int isValidMemAccess(int loc, int byte) {
	return loc < WORDS_IN_MEM 
				&& loc >= 0
				&& byte < BYTES_PER_MEM_LOC 
				&& byte >= 0;
}

int loadMem(int loc, int byte) {
	if(isValidMemAccess(loc, byte)) {
		return MEMORY[loc][byte];
	}
	else
		exit(SIM_ERROR);
}

int loadWord(int word) {
	int littleEnd = loadMem(word, LITTLE_END);
	int bigEnd = loadMem(word, BIG_END);
	/* assume word has been stored correctly */ 
	return littleEnd | LeftShiftOneByte(bigEnd);
}

void storeMem(int loc, int byte, int value) {
	if(isValidMemAccess(loc,byte)) {
		MEMORY[loc][byte] = value; /* TODO: mask value according to how mem is stored */
	}
	else
	exit(SIM_ERROR);
}

void storeWord(int word, int wordVal) {
	int littleEnd = Low8bits(wordVal); /* protect our memory */
	int bigEnd = Low8bits(RightShiftOneByte(wordVal));
	storeMem(word, LITTLE_END, littleEnd);
	storeMem(word, BIG_END, bigEnd);
}

int xor(int val1, int val2) {
	return val1 ^ val2;
}

int add(int val1, int val2) {
	return val1 + val2;
}

int and(int val1, int val2){
  return val1 & val2;
}

typedef int (*ALUOP_t)(int, int);
int ALU(int val1, int val2, ALUOP_t opfn) {
	return 0;
}

int fetchInstr(){
  return loadWord(loadReg(PC_REG));
}

void setCC(int result) {
	if(result < 0) {
		storeReg(N_CC, 1);
		storeReg(Z_CC, 0);
		storeReg(P_CC, 0);
	}
	else if (result == 0) {
		storeReg(N_CC, 0);
		storeReg(Z_CC, 1);
		storeReg(P_CC, 0);
	} 
	else {
		storeReg(N_CC, 0);
		storeReg(Z_CC, 0);
		storeReg(P_CC, 1);
	}
}

int signedImmN(int n, int instr) {
	int imm = ImmN(n, instr);
	if(NthBit(n-1, imm)) {
		imm |= (-1 << n);
	}
	return imm;
}

int decodeAndExecInstr(int instr) {
	int opCode = OpcodeOfInstr(instr);
	int DR, BaseR, SR, SR1, SR2, N, Z, P, PC, PCoffs, addrOffs, op, op1, op2, shiftRight, amt,
		result, doBranch, subrAddr, wordToStore, loadedVal; 
	switch(opCode) {
		case ADD:
			DR = RegHigh(instr);
			SR1 = RegMid(instr);
			op1 = loadReg(SR1);
			op2 = ABit(instr) ? ImmN(5, instr) : loadReg(RegLow(instr));
			result = add(op, op2);
			storeReg(DR, result);
			setCC(result);
			break;
		case AND:

			break;
		case BR:
			N = NBit(instr);
			Z = ZBit(instr);
			P = PBit(instr);
			doBranch = 
				(N && loadReg(N_CC)) ||
				(Z && loadReg(Z_CC)) ||
				(P && loadReg(P_CC));
			if(doBranch) {
				PCoffs = signedImmN(9, instr);
				addrOffs = PCoffs << 1;
				storeReg(PC_REG, loadReg(PC_REG) + addrOffs);
			}
			break;
		case JMP:

			break;
		case JSR:
			storeReg(7, loadReg(PC));
			subrAddr = JSR_ABit(instr) ?
				loadReg(PC_REG) + (signedImmN(11, instr) << 1) :
				loadReg(RegMid(instr));
			storeReg(PC_REG, subrAddr);
			break;
		case LDB:

			break;
		case LDW:
			DR = RegHigh(instr);
			BaseR = RegMid(instr);
			addrOffs = signedImmN(6, instr) << 1;
			loadedVal = loadWord(BaseR + addrOffs);
			storeReg(DR, loadedVal);
			setCC(loadedVal);
			break;
		case LEA:

			break;
		case SHF:
			DR = RegHigh(instr);
			SR = RegMid(instr);
			shiftRight = DBit(instr);
			op = loadReg(SR);
			amt = ImmN(4, instr);
			if(shiftRight) {
				if(ABit(instr)) { /* arithmetic */
					result = op >> amt;
				}
				else { /* logical */
					result = (op >> amt) & (1 << (16 - amt) - 1);
				}
			}
			else {
				result = op << amt;
			}
			storeReg(DR, result);
			setCC(result);
			break;
		case STB:

			break;
		case STW:
			SR1 = RegHigh(instr);
			BaseR = RegMid(instr);
			addrOffs = signedImmN(6, instr) << 1;
			wordToStore = loadReg(SR1);
			storeWord(BaseR + addrOffs, wordToStore);
			break;
		case TRAP:

			break;
		case XOR:
			DR = RegHigh(instr);
			SR1 = RegMid(instr);
			op1 = loadReg(SR1);
			op2 = ABit(instr) ?
				signedImmN(5, instr) :
				loadReg(RegLow(instr));
			result = xor(op1, op2);
			storeReg(DR, result);
			setCC(result);
			break;

	}
}

void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */     
	int instr = fetchInstr();
	decodeAndExecInstr(instr);
}

