#include <stdio.h>
#include "error.h"
#include "parse.h"
#include "num.h"

FILE* infile = NULL;
FILE* outfile = NULL;
int numLines = 0;

void assess(char * line) {
	printf("%s\n", line);
	numLines++;
}

void doForEachLine(FILE * inFile, void (*func)(char *)) {
	char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
		 *lArg2, *lArg3, *lArg4;
	int lRet;
	do
	{
		lRet = readAndParse( inFile, lLine, &lLabel,
				&lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
		if( lRet != DONE && lRet != EMPTY_LINE )
		{
			func(lLine);
		}
	} while( lRet != DONE );
}

void printOut(FILE * outFile, int * instrs, int numInstrs) {
	while(numInstrs-- > 0) {
		fprintf( outFile, "0x%.4X\n", *instrs);
		instrs++;
	}
}

void parseArgs(int argc, char* argv[]) {
	if (argc != 3) {
		printf("invalid args. example: './prog <in.asm> <out.obj>'\n");
		exit(4);
	}

	char *prgName = NULL;
	char *iFileName = NULL;
	char *oFileName = NULL;

	prgName = argv[0];
	iFileName = argv[1];
	oFileName = argv[2];

	infile = fopen(iFileName, "r");
	outfile = fopen(oFileName, "w");

	if (!infile) {
		printf("Error: Cannot open file %s\n", argv[1]);
		exit(4);
	}
	if (!outfile) {
		printf("Error: Cannot open file %s\n", argv[2]);
		exit(4);
	}
}

int main(int argc, char* argv[]) {
	parseArgs(argc, argv);

	doForEachLine(infile, assess);

	int * instrs = malloc(sizeof(int) * numLines);
	instrs[0] = 12;
	instrs[1] = 45;
	printOut(outfile, instrs, numLines);

	fclose(infile);
	fclose(outfile);
}

