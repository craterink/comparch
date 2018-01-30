#define NOCONST     0
#define NEG_ALLOWED 1
#define POS_ONLY    2

int regHigh(char * arg);
int regMed(char * arg);
int regLow(char * arg);

int immN(char * arg, int numBits, int immType);

