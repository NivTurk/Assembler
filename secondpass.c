#include "secondpass.h"
/*Written by Amir Zvik & Niv Turk*/


int secondpass (FILE *code , symbol **head , symbol **tail , char *fileName){
    /*declerations*/
    int returnVal;
    if(code == NULL){
        fprintf(stdout,"file opening failed\n");
        exit(1);
    }
    else {
        rewind(code);
    }
    if(unusedLabel(*head)){
        return 1; 
    }
    returnVal = completeList(code , head , tail , fileName);

    return returnVal;
}

/*filling missing data in the symbols list return 1 for error and hundle .ext File*/
int completeList(FILE *code , symbol **head , symbol **tail  , char *fileName){
    /*declarations*/
    symbol *symbolPtr;
    pList *pListptr;
    int numOfParameter , lineNumber;
    FILE *extFile; 
    unsigned extstatus , errstatus;
    char buffer[LINE_SIZE] , seconedBuffer[LABEL_SIZE] , *ptr,*extname;

    errstatus = 0 ;
    extstatus = 0 ;
    extname = (char *)malloc((strlen(fileName)+4)*sizeof(char));
    if(extname == NULL){
        fprintf(stdout ,".ext file name allocation failure exiting prog\n");
        exit(2);
    }
    symbolPtr= *head;
    strcpy(extname, fileName); 
    strcat(extname, ".ext");
    extFile = fopen(extname, "w");
    if(extFile == NULL){
        fprintf(stdout ,".ext file failure exiting prog\n");
        exit(2);
    }
    lineNumber = 0 ;
    /*running on the file + table*/
    while(fgets(buffer, LINE_SIZE , code)){
        lineNumber++;
        numOfParameter = 0 ; 
        ptr = buffer; 
        nextword(&ptr);
        if(ptr[0] == ';'){ /*ignore an comment line*/
            continue; 
        }
        if(strstr(buffer, ".entry")  == NULL && strstr(buffer , ".extern") == NULL ){
            char *follow ;
            follow = buffer ;
            nextword(&follow);
            if(strlen(follow) <=1){ /*skip an empty line*/
                continue; 
            }
            if(symbolPtr->head != NULL){
                pListptr = symbolPtr->head ; 
                /*if a label exist at the start of a line skip it*/
                if(symbolPtr->label != NULL){
                    skipWord(&follow);
                }
                /*skip command*/
                nextword(&follow);
                skipWord(&follow);
                pListptr= pListptr->next ; 
                /*parameters*/
                while(pListptr != NULL){
                    numOfParameter++;
                    if(pListptr->bcode[0] == -1){
                        symbol *temp; 
                        moveinbuf(&follow);
                        /*find parameter THAT IS A LABEL*/
                        copyName(seconedBuffer , follow);
                        temp = lableSearch(*head, seconedBuffer);
                        if(temp != NULL){
                            if(temp->status == EXT){ /*if found a call for extern label put it in .ext file */
                                extstatus = 1; 
                                hundleARE(pListptr->bcode ,EXT);
                                hundleAdress(pListptr->bcode,temp->location);
                                fprintf(extFile ,"%s\t\t%s\n" , temp->label ,(ptr = adressto32(pListptr->adress)) );
                                free(ptr);
                            }else{
                                hundleARE(pListptr->bcode ,ENT);
                                hundleAdress(pListptr->bcode,temp->location);
                            }
                        }else{
                            errstatus++; 
                            fprintf(stdout, "illegal label %s not found and not extern at line number %d \n" , seconedBuffer, lineNumber);
                            pListptr = pListptr->next ;
                            continue;
                        }
                    }else{
                        nextword(&follow);
                        skipWord(&follow);
                    }
                    pListptr =pListptr->next; 
                }
            }
            symbolPtr =symbolPtr->next;
        }
        else{
            if(strstr(buffer , ".extern") != NULL ){
                symbolPtr =symbolPtr->next;
            }
        }
    }
    if(errstatus > 0){
        fclose(extFile);
        remove(extname);
        free(extname);
        printf("Second pass found an %d Errors \n" , errstatus); 
        return errstatus;
    }else{
        if(extstatus == 0 ){
            fclose(extFile); 
            remove(extname);
            free(extname); 
        }else{
            fclose(extFile);
            free(extname);
        }
    }
    return 0;


}


/*opening file by the needed name 2 for Entry , 1 for External , 0 for .ob*/
FILE *openfile(char *filename , int kind) {
    char *name ;
    FILE *new ;
    if(kind == ENT){
        char ent[5] = {".ent"} ;
        name = (char*)malloc((5+strlen(filename))*sizeof(char));
        if(name == NULL){
            fprintf(stdout, "FILE name malloc failure closing program\n") ;
            exit(2);
        }
        strcpy(name, filename) ;
        strcat(name , ent) ;
        new = fopen(name ,"w") ;
        if(new == NULL){
            fprintf(stdout, "File open failure closing program\n");
            exit(2);
        }
        free(name);
    }else if(kind == EXT){
        char ext[5] = {".ext"} ;
        name = (char*)malloc((5+strlen(filename))*sizeof(char));
        if(name == NULL){
            fprintf(stdout, "FILE name malloc failure closing program\n") ;
            exit(2);
        }
        strcpy(name, filename) ;
        strcat(name , ext) ;
        new = fopen(name ,"w") ;
        if(new == NULL){
            fprintf(stdout, "File open failure closing program\n");
            exit(2);
        }
        free(name);
    }else if(kind == OB){
        char ob[4] = {".ob"};
        name = (char*)malloc((5+strlen(filename))*sizeof(char));
        if(name == NULL){
            fprintf(stdout, "FILE name malloc failure closing program\n") ;
            exit(2);
        }
        strcpy(name, filename) ;
        strcat(name , ob) ;
        new = fopen(name ,"w") ;
        if(new == NULL){
            fprintf(stdout, "File open failure closing program\n");
            exit(2);
        }
        free(name);
    }
    else{
        return NULL;
    }
    return new;
}



/*Inserting a ARE code to a given buffer at cells 8,9 */
void hundleARE(unsigned int *buffer, int areCode){
    if(areCode == EXT){
        buffer[8] = 0;
        buffer[9] = 1; 
    }else if(areCode == ENT){
        buffer[8] = 1; 
        buffer[9] = 0; 
    }else{
        buffer[8] = 0;
        buffer[9] = 0;
    }
}



/*inserting address into cells 0-7 of buffer ,rest contain ARE*/
void hundleAdress(unsigned int *buffer , int address){
    int index = 0 , power = 128 , num = address ;
    if(num >= 0){
        while(index <NUM_WITH_ARE){
            if(num / power > 0){
                buffer[index] = 1;
                index++;
                num %= power; 
                power /=2; 
            }else{
                buffer[index] = 0 ;
                power /=2 ; 
                index++; 
            }    
        }
    }
}



/*finishes files .ent , .ob*/
int finishFiles(symbol **head , int IC , int DC , char *fileName ){
    symbol *symbolPtr ; 
    pList *plistPtr ;
    FILE *entFile , *obFile ;
    unsigned isEnt ; 
    char *temp1 , *temp2; 
    isEnt = 0 ; 
    symbolPtr = *head ; 
    entFile = openfile(fileName, ENT) ; 
    if(entFile == NULL){
        fprintf(stdout , "file open finishfiles failure exiting prog\n");
        exit(2);
    }
    obFile = openfile(fileName , OB); 
    if(obFile == NULL){
        fprintf(stdout , "file open finishfiles failure exiting prog\n");
        exit(2);
    }
    fprintf(obFile , "\t%s %s\n" , (temp1 = adressto32(IC)) , (temp2= adressto32(DC))); 
    free(temp1);
    free(temp2); 
    while(symbolPtr != NULL){
        if(symbolPtr->location == 0 ){
            symbolPtr = symbolPtr->next; 
            continue ;
        }
        plistPtr = symbolPtr->head ; 
        if(symbolPtr->status == ENT){
            isEnt = 1 ;
            fprintf(entFile , "%s\t%s\n" ,symbolPtr->label ,(temp1 = adressto32(symbolPtr->location))) ;
            free(temp1); 
        }
        while(plistPtr !=NULL){
            fprintf(obFile , "%s\t\t%s\n" ,(temp2 = adressto32(plistPtr->adress)),(temp1=wordto32(plistPtr->bcode)) ) ;
            free(temp1);
            free(temp2);
            plistPtr = plistPtr->next; 
        }
        symbolPtr =symbolPtr->next; 
    }

    if(isEnt == 0 ){
        char *buf ;
        buf = (char*)malloc((5+strlen(fileName))*sizeof(char));
        if(buf == NULL){
            fprintf(stdout , "ALLOCATION FAILURE AT FINISHFILES FUNCTION\n");
            exit(2);
        }
        strcpy(buf, fileName);
        fclose(entFile); 
        remove(strcat(buf , ".ent")); 
        free(buf); 
        fclose(obFile); 
        return 1;
    }else{
        fclose(obFile);
        fclose(entFile);
        return 1;  
    }
}



int unusedLabel(symbol *head){
    symbol *ptr; 
    ptr= head; 
    while(ptr != NULL){
        if(ptr->status == ENT && ptr->location == 0 ){
            printf("An unused entry label %s found at the memory \n", ptr->label);
            return 1; 
        }
        ptr = ptr->next;
    }
    return 0 ; 
}