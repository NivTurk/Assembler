#ifndef SYMBOLS_H
#define SYMBOLS_H
#define BINSIZE 10	
#define LABEL_SIZE 31
#define INSTRUCTION 1
#define DATA 2
#define EXTERN_LABEL 0
/*Written by Amir Zvik & Niv Turk*/
typedef struct part{
	unsigned int adress;		
	unsigned int bcode[BINSIZE];
	struct part *next;
}pList;


typedef struct symbol{
	char  *label;
	unsigned status:2;
	unsigned type:2;
	unsigned int location ; 
	pList *head;
	struct symbol *next;
}symbol;

enum StatusOfDeclarations{
	REG,
	EXT,
	ENT
};
/*insertions*/

void symbolInsert(symbol **tail, symbol **head,char *label, int adress ,int status ,int type);
void pListInsert(pList **head ,int address , int *bcode) ;
void symbolDelete(symbol **head ,symbol **ptr) ;


/*free memory functions*/

void freeSymbols(symbol **head) ;
void freepList(pList *head) ;

/*search in table for a label*/
symbol *lableSearch(symbol *head, char *label) ;

void printSymbols(symbol *Shead) ; 
void printpList(pList *head); 
#endif

/*sorts for the table*/
int SymbolSort(symbol **head ,symbol **tail);
