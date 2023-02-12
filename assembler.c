#include <stdio.h>
#include <string.h>
#include"firstpass.h"
#include"preassembler.h"
#include "secondpass.h"
/*Written by Amir Zvik 314673195 & Niv Turk 316550540*/


int main(int argc,char *argv[])
{
	FILE *fileAm; 
	char* file ;
	int numoferr =0 ;
	if(argc<1){
		printf("no arguments found\n");
		exit(1);
	}
	 while (--argc > 0){
	 	symbol *head , *tail ;
		int IC= 0 , DC =0;
	 	file=(char*)malloc(strlen(argv[argc])+4*sizeof(char));
	 	if(file==NULL){
	 		printf("memory allocation falied\n");
	 		exit(1);
	 	}
		head = tail =NULL; 
	 	strcpy(file,argv[argc]);
	 	strcat(file,".as");
	 	/*printf("Working on %s\n",file); */ 
		fileAm = preassembler(file);	/*pre assembler is now returning char*(file name)*/
		
		if(fileAm == NULL){
			printf("preassembler Found an error at file %s \n", file);
			free(file); 
			continue; 
		}
		
		if((numoferr =firstpass(fileAm,&head,&tail , &IC , &DC)) == 0){
			if(secondpass(fileAm,&head,&tail , argv[argc]) ==0){
				if(SymbolSort(&head , &tail )){
					printf("Head pointer reached NULL to symbol sort \n"); 
					freeSymbols(&head);
					free(file);
					fclose(fileAm);
					continue;
				}
				finishFiles(&head , IC , DC , argv[argc]);
			}else{
				printf("Second pass found an error at file %s ,Skipping file \n" , file);
				freeSymbols(&head);
				free(file);
				fclose(fileAm);
				continue;
			}
		}
		else{
			printf("First pass found an %d errors at %s ,Skipping file \n" , numoferr,file);
			freeSymbols(&head);
			free(file);
			fclose(fileAm);
			continue;
		}
		freeSymbols(&head);
		free(file);
	}
	
    return 0;
}
