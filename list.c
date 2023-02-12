#include "linkedList.h"
/*Written by Amir Zvik & Niv Turk*/

/*Inserting a new node into the end of a given List*/
void insertNode(node **head ,char *name , char *data){
    if(*head == NULL){
        *head = (node*)malloc(sizeof(node));
        if(head == NULL){
            printf("headL Malloc err\n") ;
            exit(2);
        }
        (*head)->data = (char*)malloc((strlen(data)*sizeof(char)));
        if((*head)->data == NULL){
            printf("data Malloc err\n") ;
            exit(2);
        }
        (*head)->name = (char*)malloc((strlen(name)*sizeof(char)));
        if((*head)->name == NULL){
            printf("name Malloc err\n") ;
            exit(2);
        }
        (*head)->data = strcpy((*head)->data ,data) ;
        (*head)->name = strcpy((*head)->name ,name) ;
        (*head)->next=NULL ; 
        (*head)->prev = NULL ; 
    }else{
        node *new ,*ptr;
        
        ptr = *head ;
        new = (node*)malloc(sizeof(node)) ;
        if(new == NULL){
            printf("new Malloc err\n") ;
            exit(2);
        }
        new->data = (char*)malloc((strlen(data)*sizeof(char))) ;
        if(new->data == NULL){
            printf("List newData Malloc err\n") ;
            exit(2);
        }
        new->name = (char*)malloc((strlen(name)*sizeof(char))) ;
        if(new->name == NULL){
            printf("List newName Malloc err\n") ;
            exit(2);
        }
        while(ptr->next != NULL){
           ptr = ptr->next ; 
        }
        new->data = strcpy(new->data ,data) ;
        new->name = strcpy(new->name ,name) ;
        ptr->next = new ;  
        new->prev = ptr;
        new->next = NULL; 
    }
    
}

/*deleting a Node from a given list (rightnow not in use should use a dummy node)*/
node *deleteNode(node *given,node *head){
    if(given != head ){
        given->prev->next = given->next ;
    }
    else{
        head = given->next ;
        given->next->prev = NULL ;
    }
    return given ;
}

/*searching for a given macro name in a given list*/
node *searchMacro(char *name ,node *head){
    node *ptr ; 
    ptr = head ; 
    while(ptr->name !=name ||ptr->next != NULL ){
        ptr = ptr->next ;
    }
    if(ptr->name != name){
        return NULL;
    }
    return ptr ;
}

/*Checking if a list is empty*/
int isEmpty(node *head){
    if(head == NULL)
        return 1 ; 

    return 0 ; 
}

/*freeing a list from memory*/
int freeList(node *head){
    int num ;
    node *ptr; 
    num = 0 ; 
    ptr = head ; 
    while(ptr != NULL){
        ptr = ptr->next ;
        free(head);
        head = ptr ; 
        num++;
    }
    return num ;
}

/*printing list from start to end*/
void printList(node *head){
    node *ptr ; 
    int i ;
    i = 1 ; 
    ptr = head ;
    while(ptr != NULL){
        printf("Node Number: %d name is: %s ,and data is: %s\n",i , ptr->name , ptr->data) ; 
        ptr = ptr->next ; 
        i++; 
    }
}