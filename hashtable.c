#include "hashtable.h"
/*Written by Amir Zvik & Niv Turk*/

/*insertion into given hashtable*/
void hashInsert(char *name , char *data ,node *hashTable[]){
    int key ,hashnum ;  
    /*get the hash cell in the table*/
    key = convertstoi(name); 
    hashnum = 0 ; 
    hashnum = hashvalue(key) ;
    /*insert to the given list*/
    insertNode(&hashTable[hashnum],name , data ) ; 
}




 /* search return a wanted node or NULL */
node *hashSearch(char *name, node *hashTable[]){
    int key ;
    node *ptr ; 
    key = convertstoi(name) ; 
    key = hashvalue(key) ;
    ptr = hashTable[key] ;
    while(ptr != NULL && ptr->name != name && ptr->next !=NULL){
        ptr = ptr->next ;
    }
    return ptr ; 
}

/*freeing hashtable memories*/
int hashFree(node *hashTable[]){
    int i , j; 
    i = 0 ; 
    j= 0 ;
    while(i<SIZE){
        if(hashTable[i] != NULL){
            freeList(hashTable[i]);
            j++;
        }
        i++; 
    }
    return j ;
}

/*simple hash function */
int hashvalue(int key){
    return ((PRIMENUM*key)%SIZE) ; 
}

/*converting "string" into a number*/
int convertstoi(char *name){
    int i ,sum ;
    i = 0 ;
    sum =0 ; 
    while(i<strlen(name)){
        sum += (int)name[i] ;
        i++;
    }
    return sum;
}