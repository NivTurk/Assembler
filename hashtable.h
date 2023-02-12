#include "linkedList.h"
#define SIZE 100
#define PRIMENUM 13
/*Written by Amir Zvik & Niv Turk*/

/*hash table functions*/

int convertstoi(char *);
int hashvalue(int ) ; 
void hashInsert(char *, char * , node *hashtable[]) ;
node *hashSearch(char *,node *[] ) ; 
int hashFree(node *[]); 
