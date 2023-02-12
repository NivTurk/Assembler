#include "preassembler.h"
/*Written by Amir Zvik & Niv Turk*/

/*preassembler operating function*/
FILE *preassembler (char *argv){
        FILE *fileAs, *fileAm ;
        char *name=(char*)malloc(sizeof(argv)) ; 
        if(name==NULL){
        	fprintf(stdout,"memory allocation failed\n");
        	exit(1);
        }
        strcpy(name , argv) ;
        fileAm = getFile(name) ;
        fileAs = fopen(argv,"r") ;
        if(fileAs == NULL){
                char *temp ; 
                printf("opening file failed no such file at the folder\n") ;
                temp = fileName(argv); 
                fclose(fileAm);
                remove(temp) ;
                free(temp);  
                return NULL;
        } 
        if(fileAm != NULL){
                int row = 0; 
        	strcat(name,".am");
        	if((row =findMacro(fileAs , fileAm) )!= 0 ){
                        char *temp ; 
                         printf("At line %d\n", row) ;
                        temp = fileName(argv); 
                        fclose(fileAm);
                        remove(temp) ;
                        free(temp); 
                        fclose(fileAs); 
                        return NULL; 
        	}          
        }
        
        fclose(fileAs);
        rewind(fileAm);
        return fileAm ; 
}

