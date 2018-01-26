#include <stdlib.h>

// psuedo-ops
#define ORIG ".orig"
#define FILL ".fill"
#define END ".end"

// as given by lab doc
#define LABEL_MAX_STR_LEN 21
#define MAX_NUM_LABELS 255

void initSymTableBldr(void);

void buildSymTable(char * label, char * opcode, char * arg1,
		char * arg2, char * arg3, char * arg4);
