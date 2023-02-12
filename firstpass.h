#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "base.h"
#include "symbols.h"
#define LINE_SIZE 81
#define LABEL_SIZE 31
#define START_ADDRESS 100
#define SIZE 100
#define EMPTY_LINE -1
#define COMMENT_LINE 0
#define DATA_CALL 1
#define STRUCT_CALL 2
#define STRING_CALL 3
#define ENTRY_CALL 4
#define EXTERN_CALL 5
#define INSTRUCTION_CALL 6
#define SYS_ERR -1
#define FIND_ERR -2
#define OUT_OF_BOUND -3


/*Written by Amir Zvik & Niv Turk*/

/*MAIN FUNC*/

int firstpass(FILE * code,symbol **head,symbol **tail , int *IC, int *DC);


/*functions from firstpass*/

char *islabel(char *buffer); 
int typeofsentence(char *buffer) ;
char* extractString (char *buffer);
int opAddresing(int addresing , int op , int DOS);


/*errors handling functions in firstfunc */

void skipWord(char **buffer);

int moveinbuf(char **);
char *skipnum(char*) ;
int extratext(char *buffer);
char *moveahead(char* buffer) ;


/*heavy operations from firstpass*/

int operatebynum(char *buffer, int type , int *adress , symbol **head, symbol **tail, int *IC, int *DC );
int nonOperand(char *buffer ,symbol **tail ,int *adress , int numOfFunc, int *IC);
int twoOperands(char *buffer ,symbol **tail ,int *adress , int numOfFunc ,int *IC);
int oneOperand(char *buffer ,symbol **tail ,int *adress , int numOfFunc,int *IC );
int insertParameters(symbol **tail, int *adress , char *operator ,int kind ,int numOfOp,int *IC);
