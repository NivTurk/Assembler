#include <stdio.h>
#include <stdlib.h>
#include "firstpass.h"
#define LINE_SIZE 81
#define OB 0
/*Written by Amir Zvik & Niv Turk*/

int secondpass(FILE *code , symbol **head , symbol **tail , char *fileName);
int completeList(FILE *code , symbol **head , symbol **tail , char *fileName);
void hundleARE(unsigned int *buffer, int areCode);
void hundleAdress(unsigned int *buffer , int address);
/*file handle for ENT and EXT*/
FILE *openfile(char *filename , int kind );
int finishFiles(symbol **head , int IC , int DC , char *fileName );
int unusedLabel(symbol *head); 