#include "preassembler.h"
#define NUM_OF_SAVED_WORDS 29
#define NUM_OF_COMMANDS 5
/*Written by Amir Zvik & Niv Turk*/


char *savedWords[] = {"mov","cmp","add","sub","not","clr"
                ,"lea","inc","dec","jmp","bne",
                "get","prn","jsr","rts","hlt",
                "r0","r1","r2","r3","r4","r5","r6","r7" , ".entry" , ".string" , ".data" , ".struct" , ".extern"};


/* getting file name and make sure hes legit*/ 
FILE *getFile(char *name){
    char *point ;
    FILE *newfile ;
    point = fileName(name);
    if(name == NULL){
        return NULL ;
    }
    newfile= fopen(point , "w+");
    if(newfile == NULL){
        fprintf(stdout,"opening file failed  1\n") ;
        free(point);
        exit(4) ; 
    }
    free(point);
    return newfile; 
}



/*Thi function search for a macro and build a newfile with the macros*/
int findMacro(FILE *original,FILE *newfile){
    int length;
    char *ptr , buffer[MAX_BUFF_SIZE];
    node *hashTable[SIZE] ={NULL} ;  
    node *np ;
    length = 0 ;  
    ptr = (char*)malloc(MAX_BUFF_SIZE*sizeof(char)) ;
    if(ptr == NULL){
        fprintf(stdout,"malloc error at ptr find macro\node \n"); 
        exit(2);
    }
    length = 0 ;
    
    while(fgets(ptr , MAX_BUFF_SIZE-1 ,original)){
        length++;
        strcpy(buffer, ptr);
        ptr = stringBlanks(ptr) ; 
        if((strlen(ptr))>0 ){
            char *minibuf;
            if(ptr[0] == ';'){
                fputs(buffer , newfile);
                continue; 
            }
            minibuf = (char*)malloc(strlen(ptr)*sizeof(char)) ;
            if(minibuf == NULL){
                printf("malloc minibuff failure exiting prog \n");
                exit(2); 
            }
            findname(minibuf , ptr) ;/*copy the first word*/
            if(checkWord(ptr, hashTable)==0){
                hashFree(hashTable);
                free(minibuf); 
                free(ptr); 
                return length; 
            }
            /*searching name in hashtable*/
            np = hashSearch(minibuf , hashTable) ;   
            if(np != NULL){
                macrocat(&newfile ,np->data) ;
                free(minibuf) ;
                continue; 
            }
            free(minibuf) ;
            ptr = strtok(ptr, " \t");
            ptr = stringBlanks(ptr) ;
        }
        /*searching for first word == macro*/
        if (ptr!=NULL && (strcmp(ptr, "macro")) ==0 ){
            char *sec , buffName[31];
            sec = ptr ; 
            /*get the rest of the line*/
            sec = strtok(NULL, "\0");
            findname(buffName , sec) ;/*first word to buffname*/
            if(( sec = validName(sec)) == NULL){
                printf("Found an illegal macro name "); 
                free(ptr);
                hashFree(hashTable);
                return length; 
            }
            if(hashSearch(buffName  ,hashTable) != NULL){
                printf("Double declaration of macro name %s found ", buffName);
                hashFree(hashTable);
                free(ptr); 
                return length ;
            }
            /*extra text after macro name*/
            
            if(fill(&original , newfile , buffName , hashTable) ){
                free(ptr); 
                hashFree(hashTable);
                return length; 
            }
        }
        else if((ptr != NULL && (strcmp(ptr,"endmacro"))==0)){ 
            
        }
        else{
            fputs(buffer, newfile) ;
        }
    } 
    hashFree(hashTable) ;
    return 0 ; 
}




/*while buff points on the "macro .." and store the macro in a given hashtable return 1 if there was an error else 0 */
int fill(FILE **original , FILE *newfile, char *name, node *hashTable[]){
    char  *macrodata ,*macroname ,*ptr , buffer[LINE_LEN] ;
    int len ,i ,size; 
    
    /*finishing macro name*/
    macroname = (char*)malloc((strlen(name))*sizeof(char));
    if(macroname == NULL){
        fprintf(stdout,"malloc Error at fill\n");
        exit(2); 
    }
    findname(macroname , name);

    /*get the macro data */
    macrodata =(char*)malloc(sizeof(char)) ;
    if(macrodata == NULL){
        printf("malloc Error at fill\n");
        exit(2); 
    }
    /*copy macro lines*/
    size=1;
    i= 1; 
    while(strstr((fgets(buffer, LINE_LEN ,*original)),"endmacro") == NULL){
        char modify[LINE_LEN] = "\t"; 
        len =strlen(buffer) ; 
	    size+=len;
        macrodata =(char*)realloc(macrodata,size*sizeof(char)) ;
        if(macrodata == NULL){
            fprintf(stdout,"realloc Error at fill\n");
            exit(2); 
        }
        ptr = buffer; 
        ptr =stringBlanks(ptr);
        if(strlen(ptr)> 0){
            strcat(modify , ptr); 
            if(i == 1){
                strcpy(macrodata, modify);
            }else{
               strcat(macrodata, modify);
            }
        }else{
            strcat(macrodata,buffer);
        }
        i++;
    }
    ptr = buffer ;
    ptr = stringBlanks(ptr); 
    ptr = strtok(ptr, " \n");
    /*validation of endmacro line*/
    if(strcmp(ptr ,"endmacro") != 0){
        char secondCheck[20];
        findname(secondCheck, ptr); 
        if((strcmp(secondCheck,"endmacro\0") ) != 0){
            printf("NON Valid endmacro declarion ");
            return 1;
        }
    }
    if((ptr = strtok(NULL, "\0")) != NULL){
        ptr = stringBlanks(ptr); 
        if(!isspace(*ptr) && *ptr != '\0'){
            printf("found extra Text after \"endmacro\" ");
            return 1 ;
        }
    }
    /*inserting into hashtable*/
    hashInsert(macroname , macrodata ,hashTable);
    free(macroname);
    free(macrodata) ; 
    return 0; 
}



/*replace the macro name call to his data*/
int macrocat(FILE **newfile, char *data){
    fputs(data, (*newfile)) ;
    return (strlen(data)) ;
}


/*copies a given String from ptr into new*/
void findname(char *new , char *ptr){
    int i ; 
    i= 0; 
    while(ptr !=NULL &&!isspace(ptr[i]) && i <strlen(ptr)){
        new[i] = ptr[i]; 
        i++;
    }
    new[i] = '\0';
}

/*skipping white blanks*/
char *stringBlanks(char *ptr){
    while(isspace(ptr[0]) && ptr[0] != EOF){
        ptr++;
    }
    return ptr ; 
}

/*validation check for macro name*/
char *validName(char *macroname){
    int i,j; 
    if(!isalpha(macroname[0])){
        printf("macro name Error: saved word being used here !\n") ;
        return NULL ;
    }
    for(i=0 ;i <NUM_OF_SAVED_WORDS ;i++){
        if(strcmp(macroname , savedWords[i]) == 0 ){
            printf("macro name Error: saved word being used \n") ;
            return NULL ;
        }
        if(isspace(macroname[i])){
            for(j=i ; j<strlen(macroname);j++){
                if(!isspace(macroname[j]) && macroname[j] !='\0' && macroname[j] !=EOF ){
                    printf("found an extra text in macro name\n");
                    return NULL ;
                }
            }
            i = j ;
        }
    }
    return macroname; 
}


/*Checks if a given file name has .as int the end and returns .am insted 
    if file dosent has .am returns NULL*/
char *fileName(char *name){
    char *checker ,*point; 
    char token[2] =  "." ;
    point =(char*)malloc(sizeof(name)) ;
    if(point == NULL){
        fprintf(stdout ,"malloc failure at main\n") ;
        exit(2) ;
    }
    /*isolate the file name*/
    checker = strtok(name,token) ;
    strcpy(point, checker) ;
    /*check for .as after the file name*/
    checker = strtok(NULL, token) ;
    if(checker == NULL || strcmp(checker ,"as")!= 0){
        fprintf(stdout, "file name error\n") ;
        free(point);
        return NULL ; 
    }else{
        /*create the file name + .as instad of .am*/
        point = (char*)realloc(point , strlen(point)+3*sizeof(char));
        if(point == NULL){
            fprintf(stdout,"realloc error\n");
            exit(2);
        }
        strcat(point , ".am") ;
        return point ; 
    }
}


/*if a word dosent exist and it is not endmacro or any other single word command will return 1*/
int checkWord(char *buffer , node *hashtable[]){
    char *secbuf ,mark[30], *ptr ; 
    node *forcheck; 
    int i ;
    secbuf = (char*)malloc(LINE_LEN*sizeof(char));
    if(secbuf == NULL){
        printf("malloc error word checking exiting prog\n");
        exit(2);
    }
    /*looking for undefined single word sentence*/
    strcpy(secbuf , buffer);
    ptr = secbuf ;
    ptr=stringBlanks(ptr);
    findname(mark, ptr); /*copy first the first word*/
    ptr += strlen(mark) +1 ; /*advancing ptr after the word*/
    for(i =0 ; ptr[i] != EOF && i<strlen(ptr); i++){ /*checking for text after the first word */
        if(!isspace(ptr[i]))
            return 1; 
    }
    /*check if defined in the hashtable*/
    if((forcheck = hashSearch(mark , hashtable)) == NULL){
        if(mark !=NULL &&(strcmp(mark,"endmacro")!=0 && strcmp(mark , savedWords[13])!=0 && strcmp(mark , savedWords[14])!=0 )){
            
            for(i = 24 ; i < NUM_OF_SAVED_WORDS ; i++){
                if(strcmp(mark , savedWords[i]) ==0 ){
                    free(secbuf);
                    return 1; 
                }
            }
            if(mark[strlen(mark)-1] == ':'){
            }else{
                printf("Found an call for macro without define ");
                free(secbuf); 
                return 0 ; 
            }
        }
    }
    free(secbuf); 
    return 1; 
}

/*prints a whole file*/
void printfile(FILE *file){
	char c;
	while ((c=getc(file))!=EOF){
		printf("%c",c);
	}
}
