#include <stdlib.h>

typedef enum {UNDEFINED_LABELS=1, INVALID_OPCODES=2, INVALID_CONSTANTS=3} Error;

void error(Error errnum);
