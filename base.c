#include "base.h"
/*Written by Amir Zvik & Niv Turk*/


/*declaration for names of a given base , registers and functions to be used in this file only*/
char base32[]= {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v' } ;
char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};

char *functions[] = {"mov","cmp","add","sub","not","clr"
                ,"lea","inc","dec","jmp","bne","get","prn","jsr","rts","hlt"};
char *calls[] = {".entry" , ".string" , ".data" , ".struct" , ".extern"} ;



/*adresses from 100 to 500 not more then 16 spaces ;*/
char *adressto32(int address){
    char *buf ;
    int index ; 
    buf = (char *)malloc(5*sizeof(char));
    index = 0 ; 
    if(buf ==NULL){
        fprintf(stdout , "MALLOC FAILURE at BASE\n");
        exit(2); 
    }
    while(address > 0){
        buf[index] = base32[(address%BASE)] ;
        address /= BASE; 
        index++ ; 
    }
    buf[index] = '\0';
    buf = turnString(buf) ;  
    return buf ;
}

/*translates an "a 10 digit binary " to base32 string with \0 at the end*/
char *wordto32(unsigned int *word){
    char *buf; 
    int index , power =0, value = 0;
    buf = (char *)malloc(3*sizeof(char)); 
    if(buf ==NULL){
        fprintf(stdout , "MALLOC FAILURE at BASE\n");
        exit(2); 
    }
    
    index = 4 ;
    while(index >= 0){
        value += (int)(word[index]*pow(2, power));
        index --; 
        power++; 
    }
    buf[0] = base32[value]; 
    value = 0 ; 
    index = 9 ;
    power = 0 ; 
    while(index >= 5){
        value += (int)(word[index]*pow(2, power));
        index --; 
        power++ ; 
    }
    buf[1] = base32[value];
    buf[2]= '\0' ;
    
    return buf ; 
    
}




/*turn around a given string*/
char *turnString(char *string){
    int n ,i = 0 ; 
    char temp ; 
    n = strlen(string)-1;
   /* printf("given = %s\n", string);  delete*/
    while(i < n){
        temp = string[n] ;
        string[n] = string[i]; 
        string[i] = temp ; 
        i++;
        n--;
    }
    /*printf("finished = %s\n" , string) ;delete*/
    return string; 
}



/*translate an char to base32*/
char *letterto32(char letter){
    char *buf ; 
    int val ; 
    val = (int)letter ;
    buf = adressto32(val); 
    return buf ; 
}

/*translate an integer number between 255 to -256 to an "binary array" of ints without ARE*/
int *numToArray(int num){
    int power , index ,*buf; 
    if(num >255 || num < -256){
        printf("number Exceeded the limits\n") ;
        return NULL;
    }
 
    power = 256 ;
    index = 1; 
    buf = (int*)malloc(SIZE_OF_WORD*sizeof(int));
    if(buf == NULL){
        printf("Binary Buffer malloc Error\n");
        exit(2); 
    }
    if(num >= 0){
        while(index <SIZE_OF_WORD){
            if(num / power > 0){
                buf[index] = 1;
                index++;
                num %= power; 
                power /=2; 
            }else{
                buf[index] = 0 ;
                power /=2 ; 
                index++; 
            }    
        }
           	
        buf[0] = 0 ; 
        return buf ;
    }else{ /*mashlim le 2*/
        num *= -1; 
        num--; 
        buf[0] = 1; 
        while(index <SIZE_OF_WORD){
            if(num / power != 0){
                buf[index] = 1;
                index++;
                num %= power; 
                power /=2; 
            }else{
                buf[index] = 0 ;
                index++; 
                power /=2; 
            }
        }
        bitwiseand(&buf,1) ;
        return buf ; 
    }
}

/*translate an integer number between 127 to -128 to an "binary array" of ints containing ARE = A*/
int *numToArrARE(int num){
    int power , index ,*buf; 
    if(num >127 || num < -128){
        printf("number %d Exceeded the limits ", num) ;
        return NULL;
    }
    power = 64 ;
    index = 1; 
    buf = (int*)malloc(SIZE_OF_WORD*sizeof(int));
    if(buf == NULL){
        printf("Binary Buffer malloc Error\n");
        exit(2); 
    }
    if(num >= 0){
        while(index <NUM_WITH_ARE){
            if(num / power > 0){
                buf[index] = 1;
                index++;
                num %= power; 
                power /=2; 
            }else{
                buf[index] = 0 ;
                power /=2 ; 
                index++; 
            }    
        }
        buf[8] = 0 ;
        buf[9] = 0 ;	
        buf[0] = 0 ; 
        return buf ;
    }else{ /*mashlim le 2*/
        num *= -1; 
        num--; 
        buf[0] = 1; 
        while(index <NUM_WITH_ARE){
            if(num / power != 0){
                buf[index] = 1;
                index++;
                num %= power; 
                power /=2; 
            }else{
                buf[index] = 0 ;
                index++; 
                power /=2; 
            }
        }
        bitwiseand(&buf , 2) ;
        buf[8] = 0 ;
        buf[9] = 0 ;
        return buf ; 
    }
}

/*Does a BitWise & on a given *buf of int*/
void bitwiseand(int **buf,int status){
    if(status ==1){
        int index = 1 ; 
        while(index <SIZE_OF_WORD){
            if((*buf)[index] == 1){
                (*buf)[index]=0;
            }else{
                (*buf)[index] = 1; 
            }
            index++; 
        }
    }else if(status ==2 ){
        int index = 1 ; 
        while(index <NUM_WITH_ARE){
            if((*buf)[index] == 1){
                (*buf)[index]=0;
            }else{
                (*buf)[index] = 1; 
            }
            index++; 
        }
    }

}
 
 /*Functions for Instructions !*/

/*return the number of function that called*/
int findTypeOfCall(char *name){
    /*printf("name = %s,\n",name); */
	if(strcmp(name , functions[0]) ==0){
        return MOV; 
    }else if(strcmp(name , functions[1])==0){
        return CMP ;
    }else if(strcmp(name , functions[2])==0){
        return ADD; 
    }else if(strcmp(name , functions[3])==0){
        return SUB ;
    }else if(strcmp(name , functions[4])==0){
        return NOT; 
    }else if(strcmp(name , functions[5])==0){
        return CLR;
    }else if(strcmp(name , functions[6])==0){
        return LEA;
    }else if(strcmp(name , functions[7])==0){
        return INC;
    }else if(strcmp(name , functions[8])==0){
        return DEC;
    }else if(strcmp(name , functions[9])==0){
        return JMP;
    }else if(strcmp(name , functions[10])==0){
        return BNE;
    }else if(strcmp(name , functions[11])==0){
        return GET;
    }else if(strcmp(name , functions[12])==0){
        return PRN;
    }else if(strcmp(name , functions[13])==0){
        return JSR;
    }else if(strcmp(name , functions[14])==0){
        return RTS;
    }else if(strcmp(name , functions[15])==0){
        return HLT;
    }else{
        return -1 ; 
    }
}

/*This function inserts the opCode of a given number of function into the given *buffer of INT*/
void opCodeHandle(int **buffer, int numOfCode){
	int *buff;
    memset(*buffer,0,SIZE_OF_WORD*(sizeof(int)));
    buff = *buffer ;
    if(numOfCode == MOV){
		buff[0]= 0;
		buff[1]= 0;
		buff[2]= 0;
		buff[3]= 0;
	}else if(numOfCode == CMP ){
		buff[0]= 0;
		buff[1]= 0;
		buff[2]= 0;
		buff[3]= 1;
	}else if(numOfCode == ADD){
		buff[0]= 0;
		buff[1]= 0;
		buff[2]= 1;
		buff[3]= 0;
	}else if(numOfCode == SUB){
		buff[0]= 0;
		buff[1]= 0;
		buff[2]= 1;
		buff[3]= 1;
	}else if(numOfCode == NOT){
		buff[0]= 0;
		buff[1]= 1;
		buff[2]= 0;
		buff[3]= 0;
	}else if(numOfCode == CLR){
		buff[0]= 0;
		buff[1]= 1;
		buff[2]= 0;
		buff[3]= 1;
	}else if(numOfCode == LEA){
		buff[0]= 0;
		buff[1]= 1;
		buff[2]= 1;
		buff[3]= 0;
	}else if(numOfCode == INC){
		buff[0]= 0;
		buff[1]= 1;
		buff[2]= 1;
		buff[3]= 1;
	}else if(numOfCode == DEC){
		buff[0]= 1;
		buff[1]= 0;
		buff[2]= 0;
		buff[3]= 0;
	}else if(numOfCode == JMP){
		buff[0]= 1;
		buff[1]= 0;
		buff[2]= 0;
		buff[3]= 1;
	}else if(numOfCode == BNE){
		buff[0]= 1;
		buff[1]= 0;
		buff[2]= 1;
		buff[3]= 0;
	}else if(numOfCode == GET ){
		buff[0]= 1;
		buff[1]= 0;
		buff[2]= 1;
		buff[3]= 1;
	}else if(numOfCode == PRN ){
		buff[0]= 1;
		buff[1]= 1;
		buff[2]= 0;
		buff[3]= 0;
	}else if(numOfCode == JSR ){
		buff[0]= 1;
		buff[1]= 1;
		buff[2]= 0;
		buff[3]= 1;
	}else if(numOfCode == RTS ){
		buff[0]= 1;
		buff[1]= 1;
		buff[2]= 1;
		buff[3]= 0;
	}else if(numOfCode == HLT){
		buff[0]= 1;
		buff[1]= 1;
		buff[2]= 1;
		buff[3]= 1;
	}
}


/*inserting the number of a give addressing (numOfAdd) into array in binary order into opNum location 1 for source , 2 for dest*/
void typeOfAdressing(int *buff, int numOfAdd , int opNum ){
    if(opNum == 1){
        if(numOfAdd == IMMIDATE_ADDRESSING){
            buff[4] = 0 ; 
            buff[5] = 0 ; 
        }else if(numOfAdd == LABEL_ADDRESSING){
            buff[4] = 0 ; 
            buff[5] = 1 ;
        }else if(numOfAdd == STRUCT_ADDRESSING){
            buff[4] = 1 ; 
            buff[5] = 0 ; 
        }else if(numOfAdd == REG_ADDRESSING){
            buff[4] = 1 ; 
            buff[5] = 1 ;
        }
    }else if(opNum == 2){
        if(numOfAdd == IMMIDATE_ADDRESSING){
            buff[6] = 0 ; 
            buff[7] = 0 ; 
        }else if(numOfAdd == LABEL_ADDRESSING){
            buff[6] = 0 ; 
            buff[7] = 1 ;
        }else if(numOfAdd == STRUCT_ADDRESSING){
            buff[6] = 1 ; 
            buff[7] = 0 ; 
        }else if(numOfAdd == REG_ADDRESSING){
            buff[6] = 1 ; 
            buff[7] = 1 ;
        }
    }
}


/*returns the number of a given register name 0-15*/
int findReg(char *name){
    int i ; 
    if(name[0]=='r'){    
        for(i = 0 ; i<8 ; i++){
            if(strcmp(name ,reg[i]) == 0 ){
                return i ; 
            }
        }
        if(isdigit(name[1])){
            printf("%s is an undifined register, legal register number is between 0 to 7 ",name);
            return -2;
        }
    }
    return -1; 
}


/*checking if a given labelName is a legit label return 1 if is legit*/
int legalLabel(char *labelName){
    int i ; 
    if(!isalpha(labelName[0])){
        printf("Illegal first letter at Label ");
        return 0; 
    }
    if(strlen(labelName) >LABEL_SZ){
        printf("Label Length outbound ");
        return 0 ;
    }
    for(i = 0 ; i<strlen(labelName) ; i++){
        if(!isalnum(labelName[i])){
            printf("Illegal label %s", labelName);
            return 0 ;
        }
    }
    for(i = 0 ; i< KIND_OF_CALLS ; i++){
        if(strcmp(labelName, calls[i]) == 0){
            printf("Illegal label %s", labelName);
            return 0; 
        }
    }
    for(i = 0 ; i< NUM_OF_FUNCS ; i++){
        if(strcmp(labelName, functions[i])== 0){
            printf("Illegal label %s", labelName);
            return 0; 
        }
    }
    for(i = 0 ; i< NUM_OF_REGS ; i++){
        if(strcmp(labelName, reg[i])== 0){
            printf("Illegal label %s", labelName);
            return 0; 
        }
    }
    return 1; 
}

void copyInstruction(char *dest , char *src){
    int i  = 0 ;
    if(src[0] != '.'){
        return ; 
    } 
    for(i = 0; !isspace(src[i]) ; i++){
        dest[i] = src[i] ; 
    }
    dest[i] = '\0';
}

/*Returns the given type of addressing of a operand's name 0-3, -1 for error*/
int addressingNum(char *name){
    char *operand , *buff;
    int numOfReg , i;  
    buff = (char*)malloc(strlen(name)*sizeof(char));
    if(buff == NULL){
        printf("allocation failure at addressingNum\n");
        exit(2);
    }
    for(i =0 ; (!isspace(name[i]) || name[i] =='.') && i<strlen(name) && name[i] != ','; i++){
        buff[i] = name[i]; 
    }
    buff[i] = '\0';
    operand = buff ;
	if(operand[0] == '#'){
        free(buff);
        return 0 ; 
	}
	else if((operand = strchr(buff,'.')) != NULL){
        int num;
        operand++;
        if(isInteger(operand)){
            num = atoi(operand); 
            if(num >2 || num < 1){
                printf("Struct num of field out of bounds ");
                free(buff);
                return -1;
            }
        }else{
            printf("illegal field of structure ");
            free(buff);
            return -1; 
        }
        free(buff);
        return 2 ; 
	}else if((numOfReg = findReg(buff))!= -1){
        if(numOfReg==-2){ /*meaning that source conains an call to ilegal register*/
            free(buff);
            return -1;
        }
        free(buff);
        return 3; 		
	}
	else if(legalLabel(buff)){
        free(buff);
        return 1; 
	}
	else{
		printf("not any kind of addressing ");
        free(buff);
        return -1; 
	}

}

/*copy the first word of a given characters String*/
void copyName(char *dest , char *source){
    if(source != NULL && dest!= NULL){
        int i = 0 ; 
        while(source[i] != '\0' &&i < strlen(source) && !isspace(source[i]) && source[i] != '.' && source[i] !=','){
            dest[i] = source[i] ;
            i++;
        }
        dest[i] = '\0' ;
    }
}

/*copy the first word of a given characters String*/
void copyStruct(char *dest , char *source){
    if(source != NULL && dest!= NULL){
        int i = 0 ; 
        while(source[i] != '\0' &&i < strlen(source) && (!isspace(source[i]) || source[i] != '.' )&& source[i] !=','){
            dest[i] = source[i] ;
            i++;
        }
        dest[i] = '\0' ;
    }
}


/*advancing pointer str thru the blanks*/
void nextword(char **str)
{
	while (isspace(**str))
	{
		(*str)++;
	}
}



/*return 1 if a given string in s is a integer , 0 for false*/
int isInteger(char *s){
    int i;
    nextword(&s);
    if(!isdigit(s[0]) && s[0] != '+' && s[0] != '-'  ){
        return 0; 
    }
    s++; 
    if(strlen(s) > 1)
        for(i = 0; !isspace(s[i]) && s[i] != ',' && s[i]!= '\0' ; i++){
            if ((s[i] == '.' || !isdigit(s[i])) )
                  return 0;
        }


    return 1;
}



/*copy a number as a string from source to dest*/
void copyNumber(char *dest , char *source){
    if(source != NULL && dest!= NULL){
        int i = 0 ; 
        while(source[i] != '\0' &&i < strlen(source) && !isspace(source[i])){
            dest[i] = source[i] ;
            i++;
        }
        dest[i] = '\0' ;
    }
}


/*inserting an binary code for a given number of register in numOfOperand 1 for source ,2 for destination*/
int regiterBuf(int numOfReg ,int numOfOperand, int **buffer){
    int *buff ;
    buff = *buffer ;
    if(numOfOperand == SRC){ /*source*/
        if(numOfReg == 0){
            buff[0]= 0; 
            buff[1]= 0;
            buff[2]= 0;
            buff[3]= 0;
            return 0; 
        }else if(numOfReg ==1){
            buff[0]= 0; 
            buff[1]= 0;
            buff[2]= 0;
            buff[3]= 1;
            return 0; 
        }else if(numOfReg == 2){
            buff[0]= 0; 
            buff[1]= 0;
            buff[2]= 1;
            buff[3]= 0;
            return 0; 
        }else if(numOfReg == 3){
            buff[0]= 0; 
            buff[1]= 0;
            buff[2]= 1;
            buff[3]= 1;
            return 0; 
        }else if(numOfReg == 4){
            buff[0]= 0; 
            buff[1]= 1;
            buff[2]= 0;
            buff[3]= 0;
            return 0;  
        }else if(numOfReg == 5){
            buff[0]= 0; 
            buff[1]= 1;
            buff[2]= 0;
            buff[3]= 1;
            return 0;  
        }else if(numOfReg == 6){
            buff[0]= 0; 
            buff[1]= 1;
            buff[2]= 1;
            buff[3]= 0;
            return 0;  
        }else if(numOfReg == 7){
            buff[0]= 0; 
            buff[1]= 1;
            buff[2]= 1;
            buff[3]= 1;
            return 0;  
        }else{
            return 1 ; 
        }
    }else if(numOfOperand == DEST){/*destination (target)*/
        if(numOfReg == 0){
            buff[4]= 0; 
            buff[5]= 0;
            buff[6]= 0;
            buff[7]= 0;
            return 0;  
        }else if(numOfReg ==1){
            buff[4]= 0; 
            buff[5]= 0;
            buff[6]= 0;
            buff[7]= 1;
            return 0; 
        }else if(numOfReg == 2){
            buff[4]= 0; 
            buff[5]= 0;
            buff[6]= 1;
            buff[7]= 0;
            return 0; 
        }else if(numOfReg == 3){
            buff[4]= 0; 
            buff[5]= 0;
            buff[6]= 1;
            buff[7]= 1;
            return 0; 
        }else if(numOfReg == 4){
            buff[4]= 0; 
            buff[5]= 1;
            buff[6]= 0;
            buff[7]= 0;
            return 0; 
        }else if(numOfReg == 5){
            buff[4]= 0; 
            buff[5]= 1;
            buff[6]= 0;
            buff[7]= 1;
            return 0; 
        }else if(numOfReg == 6){
            buff[4]= 0; 
            buff[5]= 1;
            buff[6]= 1;
            buff[7]= 0;
            return 0; 
        }else if(numOfReg == 7){
            buff[4]= 0; 
            buff[5]= 1;
            buff[6]= 1;
            buff[7]= 1;
            return 0; 
        }else{
            return 1 ; 
        }
    }else{
        return 1 ; 
    }
}