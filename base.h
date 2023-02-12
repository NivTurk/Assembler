#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define BASE 32
#define BINBASE 2 
#define SIZE_OF_WORD 10 
#define NUM_WITH_ARE 8
#define LABEL_SZ 30
#define NUM_OF_FUNCS 15
#define KIND_OF_CALLS 5
#define NUM_OF_REGS 7
#define SRC 1
#define DEST 2
/*opcaodes numbers*/
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define NOT 4
#define CLR 5
#define LEA 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define GET 11
#define PRN 12
#define JSR 13
#define RTS 14
#define HLT 15
/*addressing consts*/
#define IMMIDATE_ADDRESSING 0
#define LABEL_ADDRESSING 1
#define STRUCT_ADDRESSING 2
#define REG_ADDRESSING 3

/*Written by Amir Zvik & Niv Turk*/
               
/*base 32 functions*/
char *wordto32(unsigned int *);
char *adressto32(int );
char *letterto32(char) ;
char *turnString(char *) ; 
void copyName(char *dest , char *source);
void copyNumber(char *dest , char *source);
void copyStruct(char *dest , char *source);
/*functions of instructions command*/
int findTypeOfCall(char *name);
void opCodeHandle(int **buff, int numOfCode);
void typeOfAdressing(int *buff, int numOfAdd , int opNum );
int legalLabel(char *name);
int findReg(char *name);
int addressingNum(char *buffer);
int regiterBuf(int numOfReg ,int numOfOperand,int **buff);
int *numToArray(int num);
int *numToArrARE(int num);
void bitwiseand(int **buf, int status) ;
/*helpers*/
int isInteger(char *buffer) ;
void nextword(char **ptr);
void copyInstruction(char *dest , char *src); 

