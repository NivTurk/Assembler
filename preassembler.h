#include "hashtable.h"
#define LINE_LEN 81
#define MAX_BUFF_SIZE 100
#define ERROR_SIGN -2
/*Written by Amir Zvik & Niv Turk*/

/*main*/

FILE * preassembler (char *argv);


/*functions.c prototypes*/

void findname(char *new,char *ptr); 
FILE *getFile(char *name); 
char *fileName(char *name);
int fill(FILE **original, FILE *new, char *name ,node *hash[]); 
int checkWord( char *buffer , node *hash[]);
int findMacro(FILE *origin,FILE *new);
char *stringBlanks(char *ptr) ; 
int macrocat(FILE **newfile , char *macroData);
char *validName(char *macroname);
void printfile(FILE *file);
