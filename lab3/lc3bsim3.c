/*
   Name 1: Cooper Raterink
   UTEID 1: cdr2678
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Simulator                                           */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************************/
/*                                                             */
/* Files:  ucode        Microprogram file                      */
/*         isaprogram   LC-3b machine language program file    */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void eval_micro_sequencer();
void cycle_memory();
void eval_bus_drivers();
void drive_bus();
void latch_datapath_values();

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
/* Definition of the control store layout.                     */
/***************************************************************/
#define CONTROL_STORE_ROWS 64
#define INITIAL_STATE_NUMBER 18

/***************************************************************/
/* Definition of bit order in control store word.              */
/***************************************************************/
enum CS_BITS {                                                  
	IRD,
	COND1, COND0,
	J5, J4, J3, J2, J1, J0,
	LD_MAR,
	LD_MDR,
	LD_IR,
	LD_BEN,
	LD_REG,
	LD_CC,
	LD_PC,
	GATE_PC,
	GATE_MDR,
	GATE_ALU,
	GATE_MARMUX,
	GATE_SHF,
	PCMUX1, PCMUX0,
	DRMUX,
	SR1MUX,
	ADDR1MUX,
	ADDR2MUX1, ADDR2MUX0,
	MARMUX,
	ALUK1, ALUK0,
	MIO_EN,
	R_W,
	DATA_SIZE,
	LSHF1,
	CONTROL_STORE_BITS
} CS_BITS;

/***************************************************************/
/* Functions to get at the control bits.                       */
/***************************************************************/
int GetIRD(int *x)           { return(x[IRD]); }
int GetCOND(int *x)          { return((x[COND1] << 1) + x[COND0]); }
int GetJ(int *x)             { return((x[J5] << 5) + (x[J4] << 4) +
		(x[J3] << 3) + (x[J2] << 2) +
		(x[J1] << 1) + x[J0]); }
int GetLD_MAR(int *x)        { return(x[LD_MAR]); }
int GetLD_MDR(int *x)        { return(x[LD_MDR]); }
int GetLD_IR(int *x)         { return(x[LD_IR]); }
int GetLD_BEN(int *x)        { return(x[LD_BEN]); }
int GetLD_REG(int *x)        { return(x[LD_REG]); }
int GetLD_CC(int *x)         { return(x[LD_CC]); }
int GetLD_PC(int *x)         { return(x[LD_PC]); }
int GetGATE_PC(int *x)       { return(x[GATE_PC]); }
int GetGATE_MDR(int *x)      { return(x[GATE_MDR]); }
int GetGATE_ALU(int *x)      { return(x[GATE_ALU]); }
int GetGATE_MARMUX(int *x)   { return(x[GATE_MARMUX]); }
int GetGATE_SHF(int *x)      { return(x[GATE_SHF]); }
int GetPCMUX(int *x)         { return((x[PCMUX1] << 1) + x[PCMUX0]); }
int GetDRMUX(int *x)         { return(x[DRMUX]); }
int GetSR1MUX(int *x)        { return(x[SR1MUX]); }
int GetADDR1MUX(int *x)      { return(x[ADDR1MUX]); }
int GetADDR2MUX(int *x)      { return((x[ADDR2MUX1] << 1) + x[ADDR2MUX0]); }
int GetMARMUX(int *x)        { return(x[MARMUX]); }
int GetALUK(int *x)          { return((x[ALUK1] << 1) + x[ALUK0]); }
int GetMIO_EN(int *x)        { return(x[MIO_EN]); }
int GetR_W(int *x)           { return(x[R_W]); }
int GetDATA_SIZE(int *x)     { return(x[DATA_SIZE]); } 
int GetLSHF1(int *x)         { return(x[LSHF1]); }

/***************************************************************/
/* The control store rom.                                      */
/***************************************************************/
int CONTROL_STORE[CONTROL_STORE_ROWS][CONTROL_STORE_BITS];

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
   There are two write enable signals, one for each byte. WE0 is used for 
   the least significant byte of a word. WE1 is used for the most significant 
   byte of a word. */

#define WORDS_IN_MEM    0x08000 
#define MEM_CYCLES      5
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */
int BUS;	/* value of the bus */

typedef struct System_Latches_Struct{

	int PC,		/* program counter */
		MDR,	/* memory data register */
		MAR,	/* memory address register */
		IR,		/* instruction register */
		N,		/* n condition bit */
		Z,		/* z condition bit */
		P,		/* p condition bit */
		BEN;        /* ben register */

	int READY;	/* ready bit */
	/* The ready bit is also latched as you dont want the memory system to assert it 
	   at a bad point in the cycle*/

	int REGS[LC_3b_REGS]; /* register file. */

	int MICROINSTRUCTION[CONTROL_STORE_BITS]; /* The microintruction */

	int STATE_NUMBER; /* Current State Number - Provided for debugging */ 
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int CYCLE_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands.                   */
/*                                                             */
/***************************************************************/
void help() {                                                    
	printf("----------------LC-3bSIM Help-------------------------\n");
	printf("go               -  run program to completion       \n");
	printf("run n            -  execute program for n cycles    \n");
	printf("mdump low high   -  dump memory from low to high    \n");
	printf("rdump            -  dump the register & bus values  \n");
	printf("?                -  display this help menu          \n");
	printf("quit             -  exit the program                \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

	eval_micro_sequencer();   
	cycle_memory();
	eval_bus_drivers();
	drive_bus();
	latch_datapath_values();

	CURRENT_LATCHES = NEXT_LATCHES;

	CYCLE_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles.                 */
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
/* Purpose   : Simulate the LC-3b until HALTed.                 */
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
	printf("Cycle Count  : %d\n", CYCLE_COUNT);
	printf("PC           : 0x%.4x\n", CURRENT_LATCHES.PC);
	printf("IR           : 0x%.4x\n", CURRENT_LATCHES.IR);
	printf("STATE_NUMBER : 0x%.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
	printf("BUS          : 0x%.4x\n", BUS);
	printf("MDR          : 0x%.4x\n", CURRENT_LATCHES.MDR);
	printf("MAR          : 0x%.4x\n", CURRENT_LATCHES.MAR);
	printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
	printf("Registers:\n");
	for (k = 0; k < LC_3b_REGS; k++)
		printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
	printf("\n");

	/* dump the state information into the dumpsim file */
	fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
	fprintf(dumpsim_file, "-------------------------------------\n");
	fprintf(dumpsim_file, "Cycle Count  : %d\n", CYCLE_COUNT);
	fprintf(dumpsim_file, "PC           : 0x%.4x\n", CURRENT_LATCHES.PC);
	fprintf(dumpsim_file, "IR           : 0x%.4x\n", CURRENT_LATCHES.IR);
	fprintf(dumpsim_file, "STATE_NUMBER : 0x%.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
	fprintf(dumpsim_file, "BUS          : 0x%.4x\n", BUS);
	fprintf(dumpsim_file, "MDR          : 0x%.4x\n", CURRENT_LATCHES.MDR);
	fprintf(dumpsim_file, "MAR          : 0x%.4x\n", CURRENT_LATCHES.MAR);
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
/* Procedure : init_control_store                              */
/*                                                             */
/* Purpose   : Load microprogram into control store ROM        */ 
/*                                                             */
/***************************************************************/
void init_control_store(char *ucode_filename) {                 
	FILE *ucode;
	int i, j, index;
	char line[200];

	printf("Loading Control Store from file: %s\n", ucode_filename);

	/* Open the micro-code file. */
	if ((ucode = fopen(ucode_filename, "r")) == NULL) {
		printf("Error: Can't open micro-code file %s\n", ucode_filename);
		exit(-1);
	}

	/* Read a line for each row in the control store. */
	for(i = 0; i < CONTROL_STORE_ROWS; i++) {
		if (fscanf(ucode, "%[^\n]\n", line) == EOF) {
			printf("Error: Too few lines (%d) in micro-code file: %s\n",
					i, ucode_filename);
			exit(-1);
		}

		/* Put in bits one at a time. */
		index = 0;

		for (j = 0; j < CONTROL_STORE_BITS; j++) {
			/* Needs to find enough bits in line. */
			if (line[index] == '\0') {
				printf("Error: Too few control bits in micro-code file: %s\nLine: %d\n",
						ucode_filename, i);
				exit(-1);
			}
			if (line[index] != '0' && line[index] != '1') {
				printf("Error: Unknown value in micro-code file: %s\nLine: %d, Bit: %d\n",
						ucode_filename, i, j);
				exit(-1);
			}

			/* Set the bit in the Control Store. */
			CONTROL_STORE[i][j] = (line[index] == '0') ? 0:1;
			index++;
		}

		/* Warn about extra bits in line. */
		if (line[index] != '\0')
			printf("Warning: Extra bit(s) in control store file %s. Line: %d\n",
					ucode_filename, i);
	}
	printf("\n");
}

/************************************************************/
/*                                                          */
/* Procedure : init_memory                                  */
/*                                                          */
/* Purpose   : Zero out the memory array                    */
/*                                                          */
/************************************************************/
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

/***************************************************************/
/*                                                             */
/* Procedure : initialize                                      */
/*                                                             */
/* Purpose   : Load microprogram and machine language program  */ 
/*             and set up initial state of the machine.        */
/*                                                             */
/***************************************************************/
void initialize(char *ucode_filename, char *program_filename, int num_prog_files) { 
	int i;
	init_control_store(ucode_filename);

	init_memory();
	for ( i = 0; i < num_prog_files; i++ ) {
		load_program(program_filename);
		while(*program_filename++ != '\0');
	}
	CURRENT_LATCHES.Z = 1;
	CURRENT_LATCHES.STATE_NUMBER = INITIAL_STATE_NUMBER;
	memcpy(CURRENT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[INITIAL_STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);

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
	if (argc < 3) {
		printf("Error: usage: %s <micro_code_file> <program_file_1> <program_file_2> ...\n",
				argv[0]);
		exit(1);
	}

	printf("LC-3b Simulator\n\n");

	initialize(argv[1], argv[2], argc - 2);

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

   CONTROL_STORE
   MEMORY
   BUS

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */
/***************************************************************/

#define Highbit(x) (((x) & 0x8000) >> 15)
#define isNeg(x) (((x) < 0) || (Highbit((x))))
#define Lowbit(x) ((x) & 0x1)
#define Low8bits(x) ((x) & 0xFF)
#define High8bits(x) (((x) & 0xFF00) >> 8)
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
#define N_CC   -2
#define Z_CC   -3
#define P_CC   -4

#define BYTES_PER_MEM_LOC 2
#define LITTLE_END        0
#define BIG_END           1

#define ALUK_ADD 0
#define ALUK_AND 1
#define ALUK_XOR 2
#define ALUK_PASSA 3

#define PCMUX_INCR2 0
#define PCMUX_BUS 1
#define PCMUX_ADDER 2

#define DRMUX_HIGH 0
#define DRMUX_R7 1

#define SR1MUX_HIGH 0
#define SR1MUX_MID 1

#define ADDR1MUX_PC 0
#define ADDR1MUX_BASER 1

#define ADDR2MUX_ZERO 0
#define ADDR2MUX_OFF6 1
#define ADDR2MUX_OFF9 2
#define ADDR2MUX_OFF11 3

#define MARMUX_TRAP 0
#define MARMUX_ADDER 1

#define RW_RD 0
#define RW_WR 1

#define DATA_SIZE_BYTE 0
#define DATA_SIZE_WORD 1

#define ADD  1
#define AND  5
#define BR   0
#define JMP  12
#define JSR  4
#define LDB  2
#define LDW  6
#define LEA  14
#define SHF  13
#define STB  3
#define STW  7
#define TRAP 15
#define XOR  9

int GATE_MARMUX_IN;
int GATE_ALU_IN;
int GATE_SHF_IN;
int GATE_PC_IN;
int GATE_MDR_IN;


int loadReg(int regNum){
	if(regNum >= 0 && regNum <= MAX_REG) return NEXT_LATCHES.REGS[regNum];
	else if(regNum == PC_REG)                 return NEXT_LATCHES.PC;
	else if(regNum == N_CC)                   return NEXT_LATCHES.N;
	else if (regNum == Z_CC)                  return NEXT_LATCHES.Z;
	else if (regNum == P_CC)                  return NEXT_LATCHES.P;
	else                                      exit(SIM_ERROR);
}

void storeReg(int regNum, int value){
	if(regNum >= 0 && regNum <= MAX_REG) NEXT_LATCHES.REGS[regNum] = Low16bits(value);
	else if(regNum == PC_REG)                 NEXT_LATCHES.PC = Low16bits(value);
	else if (regNum == N_CC)                  NEXT_LATCHES.N = Lowbit(value);
	else if (regNum == Z_CC)                  NEXT_LATCHES.Z = Lowbit(value);
	else if (regNum == P_CC)                  NEXT_LATCHES.P = Lowbit(value);
	else                                      exit(SIM_ERROR);
}

int isValidMemAccess(int loc, int byte){
	return loc < WORDS_IN_MEM && loc >= 0 && byte < BYTES_PER_MEM_LOC && byte >= 0;
}

int loadMem(int loc, int byte){
	if(isValidMemAccess(loc, byte)) return MEMORY[loc][byte];
	else                            exit(SIM_ERROR);
}

int loadWord(int word){
	int memIndex = word/2;
	int littleEnd = loadMem(memIndex, LITTLE_END);
	int bigEnd =    loadMem(memIndex, BIG_END);
	/* assume word has been stored correctly */ 
	return littleEnd | LeftShiftOneByte(bigEnd);
}

void storeMem(int loc, int byte, int value){
	if(isValidMemAccess(loc,byte)){
		MEMORY[loc][byte] = value; /* TODO: mask value according to how mem is stored */
	}
	else exit(SIM_ERROR);
}

void storeWord(int word, int wordVal) {
	int littleEnd = Low8bits(wordVal); /* protect our memory */
	int bigEnd    = Low8bits(RightShiftOneByte(wordVal));
	int memIndex = word / 2;
	storeMem(memIndex, LITTLE_END, littleEnd);
	storeMem(memIndex, BIG_END,    bigEnd);
}

int add(int val1, int val2){ return val1 + val2; }
int and(int val1, int val2){ return val1 & val2; }
int xor(int val1, int val2){ return val1 ^ val2; }


int signedImmN(int n, int instr){
	int imm  = ImmN(n, instr);
	if(NthBit(n-1, imm)) imm |= (-1 << n);
	return imm;
}

int signedByteToWord(int byte) {
	if(NthBit(7, byte)) {
		return (byte | 0xff00); /* sign extend */
	}
	else
		return byte;
}

void setCC(int result){
	if(isNeg(result)){
		storeReg(N_CC, 1);
		storeReg(Z_CC, 0);
		storeReg(P_CC, 0);
	}
	else if (result == 0){
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

/* 
 * Evaluate the address of the next state according to the 
 * micro sequencer logic. Latch the next microinstruction.
 */
void eval_micro_sequencer() {
	int *cuinstr = CURRENT_LATCHES.MICROINSTRUCTION; 
	int IR = CURRENT_LATCHES.IR;
	if(GetIRD(cuinstr)) {
		NEXT_LATCHES.MICROINSTRUCTION[J5] = 0;
		NEXT_LATCHES.MICROINSTRUCTION[J4] = 0;
		NEXT_LATCHES.MICROINSTRUCTION[J3] = NthBit(15,IR);
		NEXT_LATCHES.MICROINSTRUCTION[J2] = NthBit(14,IR);
		NEXT_LATCHES.MICROINSTRUCTION[J1] = NthBit(13,IR);
		NEXT_LATCHES.MICROINSTRUCTION[J0] = NthBit(12,IR);
	}
	else {
		NEXT_LATCHES.MICROINSTRUCTION[J5] = 
			CURRENT_LATCHES.MICROINSTRUCTION[J5];
		NEXT_LATCHES.MICROINSTRUCTION[J4] = 
			CURRENT_LATCHES.MICROINSTRUCTION[J4];
		NEXT_LATCHES.MICROINSTRUCTION[J3] = 
			CURRENT_LATCHES.MICROINSTRUCTION[J3];

		int branch = NthBit(1,GetCOND(cuinstr)) 
			&& !NthBit(0, GetCOND(cuinstr))
			&& CURRENT_LATCHES.BEN;
		int ready = !NthBit(1,GetCOND(cuinstr)) 
			&& NthBit(0, GetCOND(cuinstr))
			&& CURRENT_LATCHES.READY;
		int addrMode = NthBit(1,GetCOND(cuinstr)) 
			&& NthBit(0, GetCOND(cuinstr))
			&& NthBit(11, IR);
		NEXT_LATCHES.MICROINSTRUCTION[J2] = branch || CURRENT_LATCHES.MICROINSTRUCTION[J2];
		NEXT_LATCHES.MICROINSTRUCTION[J1] = ready || CURRENT_LATCHES.MICROINSTRUCTION[J1];
		NEXT_LATCHES.MICROINSTRUCTION[J0] = addrMode || CURRENT_LATCHES.MICROINSTRUCTION[J0];
	}
	int *n = NEXT_LATCHES.MICROINSTRUCTION;
	NEXT_LATCHES.STATE_NUMBER = n[J0] + n[J1]*2 + n[J2]*4 + n[J3]*8 + n[J4]*16 + n[J5]*32;
	memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[NEXT_LATCHES.STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);
}


/* 
 * This function emulates memory and the WE logic. 
 * Keep track of which cycle of MEMEN we are dealing with.  
 * If fourth, we need to latch Ready bit at the end of 
 * cycle to prepare microsequencer for the fifth cycle.  
 */
#define LC3_MEM_CYCLES 5
int cycle_num = 0;
void cycle_memory() {
	int *cui = CURRENT_LATCHES.MICROINSTRUCTION;
	if(GetMIO_EN(cui)) {
		if(++cycle_num == LC3_MEM_CYCLES - 1) {
			/* latch ready bit */
			NEXT_LATCHES.READY = 1;
		} else {
			NEXT_LATCHES.READY = 0;
		}

	} else {
		cycle_num = 0;
	}
}


/* 
 * Datapath routine emulating operations before driving the bus.
 * Evaluate the input of tristate drivers 
 *             Gate_MARMUX,
 *		 Gate_PC,
 *		 Gate_ALU,
 *		 Gate_SHF,
 *		 Gate_MDR.
 */   
int SR1_OUT, SR2_OUT, SR2MUX_OUT;
int MARMUX_LIN, MARMUX_RIN;
int ADDR2MUX_OUT, ADDR1MUX_OUT, ADDRADDER_OUT;
int PCMUX_LIN, PCMUX_MIN, PCMUX_RIN, PCMUX_OUT;
void eval_bus_drivers() {
	int *cuinstr = CURRENT_LATCHES.MICROINSTRUCTION; 
	int IR = CURRENT_LATCHES.IR;
	int *REGS = CURRENT_LATCHES.REGS;
	
	SR1_OUT = REGS[
		(GetSR1MUX(cuinstr) ? RegMid(IR) : RegHigh(IR))
	];
	SR2_OUT = REGS[RegLow(IR)];
	SR2MUX_OUT = ABit(IR) ? signedImmN(5, IR) : SR2_OUT;

	switch(GetADDR2MUX(cuinstr)) {
		case ADDR2MUX_OFF11:
			ADDR2MUX_OUT = signedImmN(11, IR);
			break;
		case ADDR2MUX_OFF9:
			ADDR2MUX_OUT = signedImmN(9, IR);
			break;
		case ADDR2MUX_OFF6:
			ADDR2MUX_OUT = signedImmN(6, IR);
			break;
		case ADDR2MUX_ZERO:
			ADDR2MUX_OUT = 0;
			break;
	}

	ADDR1MUX_OUT = GetADDR1MUX(cuinstr) ? SR1_OUT : CURRENT_LATCHES.PC;

	ADDRADDER_OUT = ADDR2MUX_OUT * (GetLSHF1(cuinstr) ? 2 : 1) + ADDR1MUX_OUT;

	MARMUX_LIN = ImmN(8, IR)*2;
	MARMUX_RIN = ADDRADDER_OUT; 

	if(GetGATE_MARMUX(cuinstr)) {
		GATE_MARMUX_IN = GetMARMUX(cuinstr) ? MARMUX_RIN : MARMUX_LIN;
	}
	if(GetGATE_PC(cuinstr)) {
		GATE_PC_IN = CURRENT_LATCHES.PC;
	}
	if(GetGATE_ALU(cuinstr)) {
		int aluA = SR1_OUT;
		int aluB = SR2MUX_OUT;
		int result = 0;
		switch(GetALUK(cuinstr)) {
			case ALUK_ADD:
				result = add(aluA, aluB);
				break;
			case ALUK_AND:
				result = and(aluA, aluB);
				break;
			case ALUK_XOR:
				result = xor(aluA, aluB);
				break;
			case ALUK_PASSA:
				result = aluA;
				break;
		}
		GATE_ALU_IN = result;
	}
	if(GetGATE_SHF(cuinstr)) {
		int op = SR1_OUT;
		int amt = ImmN(4, IR);
		int shiftRight = DBit(IR);
		int result;  
		if(shiftRight){
			if(ABit(IR)){ /* arithmetic */
				result = isNeg(op) ?
					((op >> amt) + Low16bits(0xffff << (16-amt))) :
					op >> amt; 
			}
			else {           /* logical */
				result = (op >> amt) & ((1 << (16 - amt)) - 1);
			}
		}
		else {
			result = op << amt;
		}
		GATE_SHF_IN = result;
	}
	if(GetGATE_MDR(cuinstr)) {
		int isWord = GetDATA_SIZE(cuinstr);
		int mar0 = Lowbit(CURRENT_LATCHES.MAR);
		int mdr = CURRENT_LATCHES.MDR;
		if(isWord) {
			GATE_MDR_IN = mdr;
		}
		else {
			GATE_MDR_IN = mar0 ? signedByteToWord(High8bits(mdr)) 
				: signedByteToWord(Low8bits(mdr));
		}
	}
}


/* 
 * Datapath routine for driving the bus from one of the 5 possible 
 * tristate drivers. 
 */       
void drive_bus() {
	int *cuinstr = CURRENT_LATCHES.MICROINSTRUCTION; 
	BUS = 0;
	if(GetGATE_MARMUX(cuinstr)) {
		BUS = GATE_MARMUX_IN;
	}
	if(GetGATE_PC(cuinstr)) {
		BUS = GATE_PC_IN;
	}
	if(GetGATE_ALU(cuinstr)) {
		BUS = GATE_ALU_IN;
	}
	if(GetGATE_SHF(cuinstr)) {
		BUS = GATE_SHF_IN;
	}
	if(GetGATE_MDR(cuinstr)) {
		BUS = GATE_MDR_IN;
	}	

}

/* 
 * Datapath routine for computing all functions that need to latch
 * values in the data path at the end of this cycle.  Some values
 * require sourcing the bus; therefore, this routine has to come 
 * after drive_bus.
 */       
void latch_datapath_values() {
	int *cuinstr = CURRENT_LATCHES.MICROINSTRUCTION; 

	if(GetLD_IR(cuinstr)) {
		NEXT_LATCHES.IR = BUS;
	}

	if(GetLD_CC(cuinstr)) {
		setCC(BUS);	
	}

	if(GetLD_MAR(cuinstr)) {
		NEXT_LATCHES.MAR = BUS;
	}

	if(GetLD_MDR(cuinstr)) {
		int isWord = GetDATA_SIZE(cuinstr);
		if(GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION)) {
			/* need to check R/W bit? */
			if(CURRENT_LATCHES.READY) {
				NEXT_LATCHES.MDR = loadWord(CURRENT_LATCHES.MAR);
			}
		} else {
			int mar0 = Lowbit(CURRENT_LATCHES.MAR);
			if(isWord) {
				NEXT_LATCHES.MDR = Low16bits(BUS);
			} else {
				int busByte = Low8bits(BUS);
				NEXT_LATCHES.MDR = mar0 ? (busByte << 8) : busByte;
			}
		}
	}

	if(GetLD_PC(cuinstr)) {
		switch(GetPCMUX(cuinstr)) {
			case PCMUX_BUS:
				PCMUX_OUT = BUS;
				break;
			case PCMUX_ADDER:
				PCMUX_OUT = ADDRADDER_OUT;
				break;
			case PCMUX_INCR2:
				PCMUX_OUT = CURRENT_LATCHES.PC + 2;
				break;
		}
		NEXT_LATCHES.PC = PCMUX_OUT;
	}

	if(GetLD_REG(cuinstr)) {
		int DR = GetDRMUX(cuinstr) ? 7 : RegHigh(CURRENT_LATCHES.IR);
		NEXT_LATCHES.REGS[DR] = Low16bits(BUS);
	}

	if(GetLD_BEN(cuinstr)) {
		int IR = CURRENT_LATCHES.IR;
		int IR_N = NthBit(11,IR);
		int IR_Z = NthBit(10,IR);
		int IR_P = NthBit(9,IR);
		
		int N = CURRENT_LATCHES.N;
		int Z = CURRENT_LATCHES.Z;
		int P = CURRENT_LATCHES.P;

		NEXT_LATCHES.BEN = (N&&IR_N) || (Z&&IR_Z) || (P&&IR_P);
	}

	if(GetR_W(cuinstr) == RW_WR) {
		if(GetMIO_EN(cuinstr)) {
			int isWord = GetDATA_SIZE(cuinstr);
			if(isWord) {
				storeWord(CURRENT_LATCHES.MAR, CURRENT_LATCHES.MDR);
			} else {
				int mar0 = Lowbit(CURRENT_LATCHES.MAR);
				storeMem(CURRENT_LATCHES.MAR / 2, mar0,
						mar0 ? High8bits(CURRENT_LATCHES.MDR) : Low8bits(CURRENT_LATCHES.MDR));
			}
		}
	}

}
