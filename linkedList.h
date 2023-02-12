#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
/*Written by Amir Zvik & Niv Turk*/

typedef struct body{
    char *name ;
    char *data ; 
    struct body *next ;
    struct body *prev ; 
}node;

/*Linked list functions*/

int isEmpty(node *);
void insertNode(node **head,char *, char *);
node *deleteNode(node *,node *) ;
node *searchMacro(char *,node *) ; 
int freeList(node *); 
void printList(node *);

