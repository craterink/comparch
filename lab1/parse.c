#include "parse.h"
#include <regex.h>

#define OPCODE_REGEX "^(ADD|AND|BR|BRN|BRZ|BRP|BRNP|BRZP|BRNZ|BRNZP|HALT|JMP|JSR|JSRR|LDB|LDW|LEA|NOP|NOT|RET|LSHF|RSHFL|RSHFA|RTI|STB|STW|TRAP|XOR)$"
#define VALID_LABEL_REGEX "^[a-wyz0-9][a-z0-9]{0,19}$"
#define REGISTER_REGEX "^r[0-7]$"

int
readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char
** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4
)
{
   char * lRet, * lPtr;
   int i;
   if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
	return( DONE );
   for( i = 0; i < strlen( pLine ); i++ )
        pLine[i] = tolower( pLine[i] );
   
  /* convert entire line to lowercase */
   *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

   /* ignore the comments */
   lPtr = pLine;
 
   while( *lPtr != ';' && *lPtr != '\0' &&
   *lPtr != '\n' ) 
        lPtr++;

   *lPtr = '\0';
   if( !(lPtr = strtok( pLine, "\t\n ," ) ) ) 
        return( EMPTY_LINE );

   if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
   { 
	   *pLabel = lPtr;
        if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
   }
   
  *pOpcode = lPtr;

   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
   
  *pArg1 = lPtr;
   
  if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

   *pArg2 = lPtr;
   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

   *pArg3 = lPtr;

   if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

   *pArg4 = lPtr;

   return( OK );
}

int checkRegexMatch(char * regexStr, char * str) {
	if (str == NULL) return -1;
	regex_t regex;
	int ret = regcomp(&regex, regexStr, REG_EXTENDED | REG_ICASE | REG_NOSUB);
	if(ret) {
		fprintf(stderr, "Could not compile regex.\n");
		exit(4);
	}
	ret = regexec(&regex, str, 0, NULL, 0);
	regfree(&regex);
	if(!ret)
		return 1;
	else if (ret == REG_NOMATCH)
		return 0;
}

int isValidLabel(char * str) {
	return checkRegexMatch(VALID_LABEL_REGEX, str)
		&& !checkRegexMatch(REGISTER_REGEX, str);	
}

int isOpcode(char * str) {
	return checkRegexMatch(OPCODE_REGEX, str) ? 0 : -1;
}

int isValidAddr(int addr) {
	return (addr >= 0 && addr <= MAX_ADDR);
}

