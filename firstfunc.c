#include "firstpass.h"
/*Written by Amir Zvik & Niv Turk*/

/* function moving ahead inside the buffer while checking for a singel ',' */ 
int moveinbuf(char **ptr){ 
    int comma = 0 ;
    if(*ptr != NULL){
        while((isspace((*ptr)[0]) ||(*ptr)[0] == ',') && (*ptr)[0] !=EOF ){
            if((*ptr)[0] == ','){
                comma++;
            }
            (*ptr)++;
        }
        if(comma > 1 || comma == 0 ){
            return 1 ; 
        }else{
            return 0 ; 
        }
    }
    return 0 ; 
    
}

/*skipping num insinde char* */
char *skipnum(char *buf){
    char *ptr ; 
    ptr = buf ; 
    while((isdigit(ptr[0]) || ptr[0] =='+' || ptr[0] =='-') && ptr[0] != '\n'){
        ptr++;
    }
    return ptr ; 
}



/*checking if theres any extra text in a given char *ptr ,returns 1 if there is extratext and 0 for non*/
int extratext(char *ptr){
	if(ptr!=NULL&& *ptr != '\0'){
		while(*ptr!='\n' && *ptr !='\0'){
			if(!isspace(*ptr) && *ptr != '\0' && *ptr != EOF)
				return 1;
			ptr++;
		}
	}
    return 0 ;
}




/*skipping a word in a given char *pointer*/
void skipWord(char **pointer){
    if(*pointer != NULL)
        while(strlen(*pointer) > 0 && !isspace((*pointer)[0])&& (*pointer)[0] != ','){
            (*pointer)++ ;
        }
}