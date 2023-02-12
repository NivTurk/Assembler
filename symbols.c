#include "symbols.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h> 
/*Written by Amir Zvik & Niv Turk*/

/*symbol inseration to table while label can be NULL*/
void symbolInsert(symbol **tail, symbol **head,char *label, int adress ,int status ,int type){ 
	if(*tail == NULL){
		(*tail) =(symbol*)malloc(sizeof(symbol)) ;
		if((*tail)== NULL){
			printf("malloc failure at table insert closing prog\n");
			exit(2); 
		}
		(*head) = *tail ; 
		if(label == NULL){
			(*tail)->label = NULL; 
		}else{
			(*tail)->label =(char *)malloc((strlen(label))*sizeof(char)) ;
			if((*tail)->label == NULL){
				printf("malloc failure at table insert label closing prog\n");
				exit(2); 
			}
			strcpy((*tail)->label , label) ; 
		}
		(*tail)->status = status ;
 		(*tail)->location = adress ;
		(*tail)->type = type;  
		(*tail)->next = NULL ;
		(*tail)->head = NULL ;
	}else{
		symbol *node ;
		node = (symbol*)malloc(sizeof(symbol)) ;
		if(node == NULL){
			printf("malloc failure at table insert closing prog\n");
			exit(2); 
		}
		if(label == NULL){
			node->label = NULL; 
		}else{
			node->label =(char *)malloc((strlen(label))*sizeof(char)) ;
			if(node->label == NULL){
				printf("malloc failure at table insert label closing prog\n");
				exit(2); 
			}
			strcpy(node->label , label) ;
		}
		node->location = adress ; 
		node->next = NULL ;
		(*tail)->next = node ; 
		(*tail) = (*tail)->next; 
		(*tail)->head = NULL ;
		(*tail)->status = status ;
		(*tail)->type = type;  
		}
}

/*symbol freeing function*/
void freeSymbols(symbol **headList){
	
	/*printf("symbols FREEING\n"); delete*/
	while(*headList != NULL){
		symbol *ptr; 
		ptr = *headList ;
		*headList = (*headList)->next; 
		if(ptr->label != NULL){
			free(ptr->label) ;
		}
		if(ptr->head != NULL){
			freepList(ptr->head) ; 
		}
		free(ptr); 
		
	}
}

/*inserting a node into the end of a given pList List*/
void pListInsert(pList **head, int address , int *bcode){
	if((*head) == NULL){
		(*head) = (pList*)malloc(sizeof(pList));
		if(*head == NULL){
			printf("plist malloc err exiting prog\n"); 
			exit(2) ;
		}
		if(bcode != NULL){
			memcpy((*head)->bcode , bcode, BINSIZE*sizeof(int)) ;
		}else{
			(*head)->bcode[0] = -1;
		}
		
		(*head)->adress = address ; 
		(*head)->next = NULL;
	}else{
		pList *ptr = *head ; 
		while(ptr->next != NULL){
			ptr = ptr->next ; 
		}
		ptr->next =(pList*)malloc(sizeof(pList)); 
		if(ptr->next == NULL){
			printf("plist malloc err exiting prog\n"); 
			exit(2) ;
		}
		ptr = ptr->next ; 
		ptr->adress = address ;
		if(bcode != NULL){
			memcpy(ptr->bcode , bcode, BINSIZE*sizeof(int)) ;
			
		}else{
			(ptr)->bcode[0] = -1;
			(ptr)->bcode[1] = -1;
			(ptr)->bcode[2] = -1;
			(ptr)->bcode[3] = -1;
			(ptr)->bcode[4] = -1;
			(ptr)->bcode[5] = -1;
			(ptr)->bcode[6] = -1;
			(ptr)->bcode[7] = -1;
			(ptr)->bcode[8] = -1;
			(ptr)->bcode[9] = -1;
		}
		ptr->next = NULL ; 
	}
	free(bcode);
}

/*freeing pList List*/
void freepList(pList *head) {
	int i = 0; 
	/*printf("pList FREEING\n");delete*/
	while(head != NULL){
		pList *ptr; 
		ptr = head ; 
		head = head->next; 
		free(ptr); 
		i++; 
	}
	/*printf("free pList %d pList nodes\n" , i) ; delete*/
}

/*searching for a given label in a given Symbols List*/
symbol *lableSearch(symbol *head, char *label){
	symbol *ptr = head ;
	if(label !=NULL){
		while(ptr != NULL){
			if(ptr->label != NULL){
				if(label != NULL && strcmp(label , ptr->label) == 0){
					return ptr ; 
				}
			}
			ptr = ptr->next ; 
		}
	}
	return NULL ;
}

/*printing a given Symbol List from sHead to the end*/
void printSymbols(symbol *Shead){
	symbol *ptr ;
	ptr = Shead ; 
	while(ptr != NULL){
		printf("Symbol ");
		if(ptr->label != NULL){
			printf("label = %s ,",ptr->label);
		}
		printf("address = %d\n",ptr->location);
		if(ptr->head != NULL){
			printpList(ptr->head);
		}
		ptr = ptr->next ; 
	}
}


void printpList(pList *head){
	pList *ptr ;
	int i ; 
	ptr = head; 
	while(ptr != NULL){
		printf("pList ");
		printf("adress = %d ,",ptr->adress);
		printf("bcode = ");
		i= 0 ; 
		while(i<9){
			printf("%d ,",ptr->bcode[i]);
			i++; 
		}
		printf("%d\n",ptr->bcode[i]);
		ptr = ptr->next ; 
	}
}

/*this function delete only uninitialized ptr.head field symbols which here their address dosent consern us*/
void symbolDelete(symbol **head ,symbol **ptr){
	symbol *p ; 
	if((*ptr) == (*head)){ /*head of table*/
		(*head) = (*head)->next; 
		if((*ptr)->label != NULL){
			free((*ptr)->label); 
		}
		free(*ptr);
	}else{/*not head of table*/
		p = *head ; 
		while(p->next != *ptr){
			p = p->next ;
		}
		if((*ptr)->label != NULL){
			free((*ptr)->label);
		}
		p->next = (*ptr)->next ; 
		free(*ptr) ;
	}
}


/*this method makes sure that the symbols are sorted to two groups of Instructions and data and keep the ordered addresses
int the lists ,  does that in O(n)*/
int SymbolSort(symbol **head,symbol **tail){
	symbol *icList, *icListPtr , *dcList, *dcListPtr ,*ptr;
	if((*head) == NULL){
		printf("Symbols partition : HEAD IS NULL\n");
		return -1; 
	}
	icList =  NULL; 
	dcList = NULL; 
	icListPtr = NULL; 
	dcListPtr = NULL ;
	ptr = (*head);
	/*create 2 lists Data and instructions*/
	while(ptr != NULL){
		if(ptr->type == INSTRUCTION || ptr->type == EXTERN_LABEL ){ /*creating instruction and extern labels list*/
			if(icList == NULL){
				icList = ptr;
				icListPtr = icList;  
			}else{
				icListPtr->next = ptr ; 
				icListPtr = icListPtr->next;
			}
			ptr = ptr->next ;
		}else{ /*creating an data list*/
			if(dcList == NULL){
				dcList = ptr ; 
				dcListPtr = dcList;
			}else{
				dcListPtr->next = ptr ; 
				dcListPtr = dcListPtr->next ;
			}
			ptr = ptr->next ; 
		}
	}
	
	/*At the end the pointer of dcListPtr and icListPtr points to the end of the lists*/
	(*head) = icList ;
	icListPtr->next = dcList ; /*Concatenation of a lists*/
	(*tail) = dcListPtr ;
	return 0 ;
}