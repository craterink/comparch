#include <stdlib.h>

enum Error {UNDEFINED_LABELS=1, INVALID_OPCODES=2, INVALID_CONSTANTS=3, OTHER=4};

void error(enum Error errnum);
