#include "firstpass.h"

/*Written by Amir Zvik & Niv Turk*/

/*this function is operating the first pass in out assembler*/
int firstpass(FILE *code, symbol **head, symbol **tail, int *IC, int *DC)
{
	/*declarations */
	int type, adress = START_ADDRESS, num, lineNumber = 0, flag = 0;
	char *str;
	char *ptr;
	if (code == NULL)
	{
		printf("file open firstpass FAILURE !\n");
		exit(2);
	}
	else
	{
		rewind(code);
	}
	str = (char *)malloc(LINE_SIZE * sizeof(char));
	if (str == NULL)
	{
		fprintf(stdout, "memory allocation failed\n");
		exit(1);
	}
	while (fgets(str, LINE_SIZE, code))
	{ /*run throw stream line by line*/
		if (adress >= 256)
		{
			printf("Ran out of Memory storage ");
			return flag;
		}
		if(strlen(str) > LINE_SIZE){
			printf("Line outbound line length ");
			return flag++ ; 
		}
		ptr = str;
		lineNumber++;
		type = typeofsentence(ptr);
		nextword(&ptr);
		if (strlen(ptr) > 1)
		{
			num = operatebynum(ptr, type, &adress, head, tail, IC, DC);

			/*num = errorhandle(num); */
			if (num < 0)
			{
				printf("at line %d\n", lineNumber);
				flag++;
			}
		}
	}
	return flag;
}

/*finding what type of sentence we got*/
int typeofsentence(char *buffer)
{
	char *ptr;
	ptr = buffer;
	nextword(&ptr);
	if (strlen(ptr) == 0)
	{
		return EMPTY_LINE;
	}
	else if (ptr[0] == ';')
	{
		return COMMENT_LINE;
	}
	else if (strstr(ptr, ".data") != NULL)
	{
		return DATA_CALL;
	}
	else if (strstr(ptr, ".struct") != NULL)
	{
		return STRUCT_CALL;
	}
	else if (strstr(ptr, ".string") != NULL)
	{
		return STRING_CALL;
	}
	else if (strstr(ptr, ".entry") != NULL)
	{
		return ENTRY_CALL;
	}
	else if (strstr(ptr, ".extern") != NULL)
	{
		return EXTERN_CALL;
	}
	else
	{
		return INSTRUCTION_CALL; /*6 is for instructions */
	}
}

/*This function recive an number of a given type of sentence and operates by the type return -2 or -3 for error
	Note this funtion is pretty long but orderd*/
int operatebynum(char *buffer, int type, int *adress, symbol **head, symbol **tail, int *IC, int *DC)
{
	if (type == EMPTY_LINE)
	{ /*empty sentence*/
		return 0;
	}
	else if (type == COMMENT_LINE)
	{ /*a note sentence*/
		return 0;
	}
	else if (type == DATA_CALL)
	{ /*.data sentence*/
		symbol *temp;
		/*getting label*/
		char *label, *ptr = buffer, *identifier ,inst[LABEL_SIZE];
		nextword(&ptr);
		label = islabel(buffer);
		if (label != NULL)
		{
			ptr += strlen(label)+1; /*+1 is for ':'*/
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
		}

		temp = lableSearch(*head, label);
		if (temp != NULL)
		{
			if (temp->head != NULL)
			{
				printf("double declaration of the label %s error ", label);
				return FIND_ERR;
			}
			if (temp->status == ENT)
			{
				if (temp->location != 0)
				{
					printf("Double declaration of the label %s error ", label);
					;
					return FIND_ERR;
				}
				symbolInsert(tail, head, label, *adress, ENT, DATA);
				symbolDelete(head, &temp);
			}
			else if (temp->status == EXT)
			{
				printf("Illegal assignment of an extern label %s ", label);
				return FIND_ERR;
			}
		}
		else
		{
			symbolInsert(tail, head, label, *adress, REG, DATA);
		}
		/*affter getting label or empty label*/
		nextword(&ptr);
		copyInstruction(inst, ptr); 
		
		/*.data command validation*/
		if (strcmp(inst, ".data") != 0)
		{
			printf("not an .data code,  is %s ", inst);
			return FIND_ERR;
		}
		/*commas checking*/
		ptr += sizeof(".data") ;
		nextword(&ptr);
		
		if (strlen(ptr) <1){
			printf("missing parameters on .data command ");
			return FIND_ERR;
		}
		if(ptr[0] == ','){
			printf("Found an illegal commas ");
			return FIND_ERR; 
		}
		/*parameters*/
		identifier = ptr;
		while (strlen(identifier) > 0)
		{
			int val, *buff;

			/*parameter validation*/
			if ((val = isInteger(identifier)) == 0)
			{
				printf("parameter %s is not an integer ", identifier);
				return FIND_ERR;
			}
			else
			{
				val = atoi(identifier);
				buff = numToArray(val);
				if (buff == NULL)
				{
					printf("parameter error ");
					return OUT_OF_BOUND;
				}
				pListInsert(&((*tail)->head), *adress, buff);
				(*adress)++;
				(*DC)++;
			}
			identifier = skipnum(identifier);
			if (moveinbuf(&identifier))
				if (strlen(identifier) > 0)
				{
					printf("%s has a comma error ", identifier);
					return FIND_ERR;
				}
		}
		return 1;
	}


	else if (type == STRUCT_CALL){ /*struct*/
		char *label, *ptr = buffer, *identifier, *string, word[30];
		int val = 0, index;
		symbol *temp;
		string = extractString(buffer);
		if (string == NULL)
			return FIND_ERR;
		nextword(&ptr);
		label = islabel(buffer);
		if (label != NULL)
		{
			ptr += strlen(label); 
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
		}

		temp = lableSearch(*head, label);
		if (temp != NULL)
		{
			if (temp->head != NULL)
			{
				printf("double declaration of the label %s error ", label);
				return FIND_ERR;
			}
			if (temp->status == ENT)
			{
				if (temp->location != 0)
				{
					printf("Double declaration of the label %s error ", label);
					return FIND_ERR;
				}
				symbolInsert(tail, head, label, *adress, ENT, DATA);
				symbolDelete(head, &temp);
			}
			else if (temp->status == EXT)
			{
				printf("Illegal assignment of an extern label %s ", label);
				return FIND_ERR;
			}
		}
		else
		{
			symbolInsert(tail, head, label, *adress, REG, DATA);
		}
		/*affter getting label or empty label*/
		nextword(&ptr);
		identifier = strtok(ptr, " \t");
		identifier = strtok(NULL, " \t");

		/*.struct command validation*/
		if (strcmp(identifier, ".struct") != 0)
		{
			printf("not an .struct code ! is %s ", identifier);
			return FIND_ERR;
		}
		/*get the parameters*/
		ptr = strtok(NULL, "\0");
		if (ptr == NULL)
		{
			printf("missing parameters on .struct command ");
			return FIND_ERR;
		}
		nextword(&ptr); 
		
		if(ptr[0] == ','){
			printf("Found an illegal commas ");
			return FIND_ERR; 
		}
		copyNumber(word, ptr);
		/*number parameter validation*/
		if ((val = isInteger(word)) == 0)
		{
			printf("parameter %s is not an integer ", identifier);
			return FIND_ERR;
		}
		else
		{
			int *buff;
			val = atoi(word);
			buff = numToArray(val);
			if (buff == NULL)
			{
				printf("parameter error ");
				return OUT_OF_BOUND;
			}
			pListInsert(&(*tail)->head, *adress, buff);
			(*adress)++;
			(*DC)++;
		}
		ptr = skipnum(ptr);
		if (moveinbuf(&ptr))
		{
			printf("Comma Error or a missing parameter ");
			return FIND_ERR;
		}
		/*string parameter*/
		nextword(&ptr);
		if (ptr[0] != '"')
		{
			printf("Not Found an First legal double quotes ! ");
			return FIND_ERR;
		}
		/*advancing to the last double quotes*/
		ptr += (strlen(string) + 1) * sizeof(char);
		if (ptr[0] != '"')
		{
			printf("Not Found an legal Second double quotes ! ");
			return FIND_ERR;
		}
		ptr++;
		if (extratext(ptr))
		{
			printf("found an extra text ! at: %s ", ptr);
			return FIND_ERR;
		}
		identifier = string;
		index = 0;
		while (index < strlen(identifier))
		{
			pListInsert(&(*tail)->head, *adress, numToArray((int)(identifier[index])));
			(*adress)++;
			(*DC)++;
			index++;
		}
		pListInsert(&(*tail)->head, *adress, numToArray((int)'\0'));
		(*DC)++;
		(*adress)++;
		free(string);
		return 1;
	}
	else if (type == STRING_CALL)
	{ /*string*/
		char *label, *ptr = buffer, *identifier, *string;
		int index;
		symbol *temp;
		string = extractString(buffer);
		if (string == NULL)
			return FIND_ERR;

		/*getting label*/
		nextword(&ptr);
		label = islabel(ptr);
		buffer = strtok(buffer, "\n");
		if (label != NULL)
		{
			ptr += strlen(label);
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
		}
		/*checks if the label exist*/
		temp = lableSearch(*head, label);
		if (temp != NULL)
		{
			if (temp->head != NULL)
			{
				printf("double declaration of the label %s error ", label);
				return FIND_ERR;
			}
			if (temp->status == ENT)
			{
				if (temp->location != 0)
				{
					printf("Double declaration of the label %s error ", label);
					return FIND_ERR;
				}
				symbolInsert(tail, head, label, *adress, ENT, DATA);
				symbolDelete(head, &temp);
			}
			else if (temp->status == EXT)
			{
				printf("Illegal assignment of an extern label %s ", label);
				return FIND_ERR;
			}
		}
		else
		{
			symbolInsert(tail, head, label, *adress, REG, DATA);
		}
		nextword(&ptr);
		identifier = strtok(ptr, " \t");
		identifier = strtok(NULL, " \t");
		/*check for the currect command (.string)*/
		if (strcmp(identifier, ".string") != 0)
		{
			printf("not an .string code ! is %s ", identifier);
			return FIND_ERR;
		}
		identifier = strtok(NULL, "\0");
		if (identifier == NULL)
		{
			printf("missing parameters on .string command ");
			return FIND_ERR;
		}
		nextword(&identifier);
		if (identifier[0] != '"')
		{
			printf("could not found an legal double quote!  \n");
			return FIND_ERR;
		}
		/*advancing to the last double quotes*/
		identifier += (strlen(string) + 1) * sizeof(char);
		if (identifier[0] != '"')
		{
			printf("could not found an legal Second double quote! ");
			return FIND_ERR;
		}
		identifier++;
		if (extratext(identifier))
		{
			printf("Found an extraText! ");
			return FIND_ERR;
		}
		identifier = string;
		index = 0;
		while (index < strlen(identifier))
		{
			pListInsert(&(*tail)->head, (*adress)++, numToArray((int)(identifier[index])));
			index++;
			(*DC)++;
		}
		pListInsert(&(*tail)->head, *adress, numToArray((int)'\0'));
		(*adress)++;
		(*DC)++;
		if (strlen(identifier) < 3)
		{
			printf("An Empty String found ");
			return FIND_ERR;
		}
		free(string);
		return 1; /*after getting the data thatn .string should have, check for garbage and returns error or 1*/
	}
	else if (type == ENTRY_CALL)
	{ /*entry*/
		char *label, *ptr = buffer, labelName[LABEL_SIZE];
		symbol *temp;
		nextword(&ptr);
		label = islabel(buffer);
		if (label != NULL)
		{
			ptr += strlen(label) +1; /*+1 for ':'*/
			printf("Warrning this label %s is Redunt label\n", label);
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
		}
		temp = lableSearch(*head, label);
		if (temp != NULL)
		{
			if (temp->head != NULL)
			{
				fprintf(stdout, "double declaration of the label %s error ", label);
				return FIND_ERR;
			}
			if (temp->status == ENT || temp->status == EXT || temp->status == REG)
			{
				fprintf(stdout, "double declaration of the label %s error ", label);
				return FIND_ERR;
			}
		}
		nextword(&ptr);
		copyInstruction(labelName , ptr);
		/*.entry command validation*/
		if (strcmp(labelName, ".entry") != 0)
		{
			printf("not a legal .entry code ! is %s ", ptr);
			return FIND_ERR;
		}
		ptr+= (strlen(labelName)) *sizeof(char);
		nextword(&ptr);
		/*label defintion validation*/
		
		nextword(&ptr);
		if(strlen(ptr) < 1){
			printf("missing a label declaration ");
			return FIND_ERR; 
		}
		copyName(labelName, ptr);
		if (legalLabel(labelName)){
			temp = lableSearch(*head, labelName ) ;
			if(temp != NULL){
				printf("Cannot define a new entry label that was defined already ");
				return FIND_ERR; 
			}
			symbolInsert(tail, head, labelName, 0, ENT, 0);
			ptr += sizeof(char) * (strlen(labelName));
		}else{
			return FIND_ERR; 
		}

		if (extratext(ptr)){
			printf("found extra text ");
			return FIND_ERR;
		}
		return 1;
	}
	else if (type == EXTERN_CALL)
	{ /*extern*/
		char *label, *ptr = buffer, labelName[LABEL_SIZE];
		symbol *temp;
		nextword(&ptr);
		label = islabel(buffer);
		if (label != NULL)
		{
			ptr += strlen(label) +1; /*+1 for ':'*/
			printf("Warrning this label %s is Redunt label\n", label);
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
	
		}
		temp = lableSearch(*head, label);
		if (temp != NULL)
		{
			if (temp->head != NULL)
			{
				fprintf(stdout, "double declaration of the label %s error ", label);
				return FIND_ERR;
			}
			if (temp->status == ENT || temp->status == EXT || temp->status == REG)
			{
				fprintf(stdout, "double declaration of the label %s error ", label);
				return FIND_ERR;
			}
		}
		nextword(&ptr);
		copyInstruction(labelName, ptr); 
		/*.extern command validation*/
		if (strcmp(labelName, ".extern") != 0)
		{
			printf("not an .extern code ! is %s ", ptr);
			return FIND_ERR;
		}
		ptr+= (strlen(labelName)) *sizeof(char);
		nextword(&ptr);
		if(strlen(ptr)< 1){
			printf("missing a label declaration "); 
			return FIND_ERR; 
		}
		/*label defintion validation*/
		copyName(labelName, ptr);
		if (legalLabel(labelName)){
			temp = lableSearch(*head, labelName ) ;
			if(temp != NULL){
				printf("Cannot define a new extern label that was defined already ");
				return FIND_ERR; 
			}
			symbolInsert(tail, head, labelName, 0, EXT, EXTERN_LABEL);
			ptr += sizeof(char) * (strlen(labelName));
		}else{
			return FIND_ERR; 
		}

		if (extratext(ptr)){
			printf("found extra text ");
			return FIND_ERR;
		}
		return 1;
	}

	else{ /*INSTRUCTION_CALL (6)*/
		char *label, *ptr = buffer, *identifier, command[4];
		symbol *temp;
		int numOfCall;
		nextword(&ptr);
		label = islabel(ptr);
		nextword(&ptr);
		if (label != NULL){
			ptr = strtok(NULL, " \t");/*ptr += strlen(label)+1*/
			if (!legalLabel(label))
			{
				return FIND_ERR;
			}
			
			temp = lableSearch(*head, label);
			if (temp != NULL)
			{
				if (temp->head != NULL)
				{
					printf("Double declaration of the label %s error ", label);
					return FIND_ERR;
				}
				if (temp->status == ENT)
				{
					if (temp->location != 0)
					{
						printf("Double declaration of the label %s error ", label);
						return FIND_ERR;
					}
					symbolInsert(tail, head, label, *adress, ENT, INSTRUCTION);
					symbolDelete(head, &temp);
				}
				else if (temp->status == EXT)
				{
					printf("Illegal assignment of an extern label %s ", label);
					return FIND_ERR;
				}
			}
			else
			{
				symbolInsert(tail, head, label, *adress, REG, INSTRUCTION);
			}
			identifier = ptr;
			nextword(&identifier);
		}
		else
		{
			symbolInsert(tail, head, NULL, *adress, REG, INSTRUCTION);
			identifier = ptr;
			nextword(&identifier);
			identifier = strtok(ptr, " \t");
		}

		copyName(command, identifier);
		numOfCall = findTypeOfCall(command);
		
		if (numOfCall != RTS && numOfCall != HLT)
		{
			ptr = strtok(NULL, "\0");
		}
		if (numOfCall < 0)
		{
			printf("false kind of operand Or An Empty label ");
			return FIND_ERR;
		}
		else if ((numOfCall >= 0 && numOfCall <= 3) || numOfCall == 6)
		{ /*Two operands functions*/
			return twoOperands(ptr, tail, adress, numOfCall, IC);
		}
		else if ((numOfCall >= 4 && numOfCall <= 5) || (numOfCall >= 7 && numOfCall <= 13))
		{ /*one operands functions*/
			return oneOperand(ptr, tail, adress, numOfCall, IC);
		}
		else if ((numOfCall >= 14 && numOfCall <= 15))
		{ /*Non operands functions*/
			return nonOperand(ptr, tail, adress, numOfCall, IC);
		}
		else
		{

			return FIND_ERR;
		}
	}
}


/*checking if the first word in str is a legal label*/
char *islabel(char *str)
{
	char *label, *c = " \t", *ptr, temp[31];
	ptr = str;
	label = (char *)malloc(LABEL_SIZE * sizeof(char));
	if (label == NULL)
	{
		printf("label allocation error exiting prog\n");
		exit(2);
	}
	nextword(&ptr);
	strncpy(label, ptr, LABEL_SIZE);
	label = strtok(label, c);
	if (label[strlen(label) - 1] == ':')
	{
		int i;
		strcpy(temp, label);
		label = (char *)calloc(strlen(label), LABEL_SIZE * sizeof(char));
		if (label == NULL)
		{
			printf("label callocation error exiting prog\n");
			exit(2);
		}
		for (i = 0; i < strlen(temp) - 1; i++)
		{
			label[i] = temp[i];
		}
		label[i] = '\0';
		return label;
	}
	else
	{
		free(label);
		return NULL;
	}
}

/*return the string which start and ends with "", if one of the quatation marks is missing or if there is additonal quatation marks return NULL  */
char *extractString(char *buffer)
{
	char *ptr, *firstQ, *secondQ, *thirdQ;
	int i;
	if ((firstQ = (strchr(buffer, '\"'))) == NULL)
	{
		printf("String command: %s is illegal ", buffer);
		return NULL;
	}

	if ((secondQ = (strchr((firstQ + sizeof(char)), '\"'))) == NULL)
	{
		printf("String command: %s is illegal ", buffer);
		return NULL;
	}
	else
	{
		thirdQ = secondQ;
		thirdQ++;
		if (*thirdQ != EOF)
		{
			while ((thirdQ = strchr(thirdQ, '\"')) != NULL)
			{
				secondQ = thirdQ;
				thirdQ++;
			}
		}
		ptr = (char *)malloc(sizeof(char) * ((secondQ - firstQ) + 1));
		if (ptr == NULL)
		{
			fprintf(stdout, "memory allocation failed\n");
			exit(1);
		}
		firstQ++;
		for (i = 0; i < (secondQ - firstQ); i++)
		{
			ptr[i] = firstQ[i];
		}
		ptr[i] = '\0';
		return ptr;
	}
}

/*this function operates on a function number from a given order without parameters*/
int nonOperand(char *buffer, symbol **tail, int *adress, int numOfFunc, int *IC)
{
	char *ptr;
	int *buff;
	ptr = buffer;
	buff = (int *)malloc(SIZE_OF_WORD * (sizeof(int)));
	if (buff == NULL)
	{
		printf("operands allocation failure exiting prog\n");
		exit(2);
	}
	/*op code for func to buf*/
	opCodeHandle(&buff, numOfFunc);
	buff[4] = 0;
	buff[5] = 0;
	buff[6] = 0;
	buff[7] = 0;
	buff[8] = 0;
	buff[9] = 0;
	pListInsert(&(*tail)->head, (*adress)++, buff);
	(*IC)++;
	ptr = strtok(NULL, "\0");
	return (ptr != NULL) ? (extratext(ptr) ? FIND_ERR : 1) : 1;
}

/*This function operates on a given function number from given order with one operand */
int oneOperand(char *buffer, symbol **tail, int *adress, int numOfFunc, int *IC)
{
	char *ptr, *opHold;
	int kindOfAdd, *buff;
	ptr = buffer;

	nextword(&ptr);
	buff = (int *)malloc(SIZE_OF_WORD * (sizeof(int)));
	if (buff == NULL)
	{
		printf("operands allocation failure exiting prog\n");
		exit(2);
	}
	opHold = (char *)malloc((LABEL_SIZE)*(sizeof(char)));
	if (opHold == NULL)
	{
		printf("operands allocation failure exiting prog\n");
		exit(2);
	}
	/*op code for func to buf*/
	opCodeHandle(&buff, numOfFunc);
	/*get parameter name*/
	nextword(&ptr);
	/*filling buff type of addressing need to check if legal !*/
	if ((kindOfAdd = addressingNum(ptr)) == -1)
	{
		printf("illegal type of adressing %s ", opHold);
		return FIND_ERR;
	}
	if(kindOfAdd == STRUCT_ADDRESSING){
		copyStruct(opHold ,ptr);
	}else{
		copyName(opHold,ptr);
	}
	skipWord(&ptr);
	if (opAddresing(kindOfAdd, numOfFunc, DEST) == 0){
		typeOfAdressing(buff, kindOfAdd, DEST);
	}else{
		printf("illegal adressing for opcode number %d ", numOfFunc);
		return FIND_ERR;
	}

	buff[4] = 0;
	buff[5] = 0;
	buff[8] = 0;
	buff[9] = 0;

	pListInsert(&(*tail)->head, (*adress)++, buff);
	(*IC)++;
	if (insertParameters(tail, adress, opHold, kindOfAdd, DEST, IC) == -1) /*numofop here being used for registers only that why SRC*/
	{
		return FIND_ERR;
	}
	
	free(opHold);
	if (strlen(ptr) > 0)
		if (extratext(ptr))
		{
			printf("found an extra text ");
			return FIND_ERR;
		}
	return 1;
}

/*Operates on a two parameters function from given order*/
int twoOperands(char *buffer, symbol **tail, int *adress, int numOfFunc, int *IC)
{
	char *ptr, op1[30], op2[30], *opHold;
	int kindOfAdd1, kindOfAdd2, *buff;
	ptr = buffer;
	buff = (int *)malloc(SIZE_OF_WORD * (sizeof(int)));
	if (buff == NULL)
	{
		printf("operands allocation failure exiting prog\n");
		exit(2);
	}
	opHold = (char *)malloc((LABEL_SIZE + 1) * (sizeof(char)));
	if (opHold == NULL)
	{
		printf("operands allocation failure exiting prog\n");
		exit(2);
	}
	/*first parameter :*/
	opCodeHandle(&buff, numOfFunc);
	/*get FIRST parameter name*/
	nextword(&ptr);
	
	if ((kindOfAdd1 = addressingNum(ptr)) == -1)
	{
		printf("illegal type of adressing %s ", op1);
		return FIND_ERR;
	}
	if(kindOfAdd1 == STRUCT_ADDRESSING){
		copyStruct(op1 ,ptr);
	}else{
		copyName(op1,ptr);
	}
	if (opAddresing(kindOfAdd1, numOfFunc, SRC) == 0)
	{
		typeOfAdressing(buff, kindOfAdd1, SRC);
	}
	else
	{
		printf("illegal adressing for opcode number %d ", numOfFunc);
		return FIND_ERR;
	}

	skipWord(&ptr);
	if (moveinbuf(&ptr))
	{
		printf("Comma Err Missing ");
		return FIND_ERR;
	}

	/*second parameter*/
	nextword(&ptr);
	/*type of addressing need to check if legal !*/
	if ((kindOfAdd2 = addressingNum(ptr)) == -1)
	{
		printf("illegal type of adressing %s ", op2);
		return FIND_ERR;
	}
	if(kindOfAdd2 == STRUCT_ADDRESSING){/*making sure struct full name gets to destination*/
		copyStruct(op2 ,ptr);
	}else{
		copyName(op2,ptr);
	}
	if (opAddresing(kindOfAdd2, numOfFunc, DEST) == 0)
	{
		typeOfAdressing(buff, kindOfAdd2, DEST);
	}
	else
	{
		printf("illegal adressing for opcode number %d ", numOfFunc);
		return FIND_ERR;
	}

	/*covering both parameters are registers */
	if (kindOfAdd1 == REG_ADDRESSING)
	{
		if (kindOfAdd2 == REG_ADDRESSING)
		{
			/*int i ; */
			int *bcode;
			buff[8] = 0;
			buff[9] = 0;
			typeOfAdressing(buff, kindOfAdd2, DEST);

			/*inserting command*/
			pListInsert(&(*tail)->head, (*adress)++, buff);
			(*IC)++;
			/*inserting a word num2with 2 operands that is registers*/

			bcode = (int *)malloc(SIZE_OF_WORD * sizeof(int));
			if (bcode == NULL)
			{
				printf("Allocation failure regBuff\n");
				exit(2);
			}
			memset(bcode, 0, SIZE_OF_WORD * (sizeof(int)));
			if (regiterBuf(findReg(op1), SRC, &bcode))
			{
				printf("Failed inserting first reg data to buffer ");
				free(bcode);
				return FIND_ERR;
			};
			if (regiterBuf((findReg(op2)), DEST, &bcode))
			{
				printf("Failed inserting second reg data to buffer ");
				free(bcode);
				return FIND_ERR;
			}
			/*bcode freeded here*/
			bcode[8] = 0;
			bcode[9] = 0;
			free(opHold);
			pListInsert(&(*tail)->head, (*adress)++, bcode);
			(*IC)++;
			return 1;
		}
	}
	/*set seconed addresing method into buff*/
	typeOfAdressing(buff, kindOfAdd2, 2);
	buff[8] = 0;
	buff[9] = 0;
	if(kindOfAdd1 == STRUCT_ADDRESSING){
		copyStruct(opHold ,op1);
	}else{
		copyName(opHold ,op1);
	}
	pListInsert(&(*tail)->head, (*adress)++, buff);
	(*IC)++;
	
	if (insertParameters(tail, adress, opHold, kindOfAdd1, SRC, IC) == -1)
	{
		return FIND_ERR;
	}
	if(kindOfAdd2 == STRUCT_ADDRESSING){
		copyStruct(opHold ,op2);
	}else{
		copyName(opHold ,op2);
	}
	if (insertParameters(tail, adress, opHold, kindOfAdd2,DEST, IC) == -1)
	{
		return FIND_ERR;
	}
	free(opHold);
	skipWord(&ptr);
	return (ptr != NULL) ? (extratext(ptr) ? FIND_ERR : 1) : 1;
}

/*inserting operator's information to Given symbol table at the tail 
kinds are: 0 for imidiate adressing , 1 for label adressing , 2 for struct adressing , 3 for register adressing*/
int insertParameters(symbol **tail, int *adress, char *operator, int kind, int numOfOp, int *IC)
{
	if (kind == IMMIDATE_ADDRESSING)
	{
		/*immidate addresing then parameter must be an integer*/
		operator++;
		if (isInteger(operator))
		{
			int val, *bcode;
			val = atoi(operator);
			if ((bcode = numToArrARE(val)) == NULL)
			{
				return -1;
			}
			pListInsert(&(*tail)->head, (*adress)++, bcode);
			(*IC)++;
		}
		else
		{
			printf("An immidiate addressing false parameter ! ");
			return -1;
		}
	}
	else if (kind == LABEL_ADDRESSING)
	{
		/*an label addressing then parameter must be a legit label name*/
		char *temp;
		temp = (char *)malloc(strlen(operator) * sizeof(char));
		if (temp == NULL)
		{
			printf("Memory allocation failure at insertParameter\n");
			exit(2);
		}
		copyName(temp, operator);
		if (legalLabel(temp))
		{
			pListInsert(&(*tail)->head, (*adress)++, NULL);
			(*IC)++;
		}
		else
		{
			printf("An direct addressing false parameter ! ");
			free(temp);
			return -1;
		}
	}
	else if (kind == STRUCT_ADDRESSING)
	{
		int val;
		char *temp;
		temp = (char *)malloc(strlen(operator) * sizeof(char));
		if (temp == NULL)
		{
			printf("Memory allocation failure at insertParameter\n");
			exit(2);
		}
		copyName(temp, operator);
		pListInsert(&(*tail)->head, (*adress)++, NULL);
		(*IC)++;
		operator+= strlen(temp)+1; /*+1 for the '.' */
		if (isInteger(operator))
		{
			int *bcode;
			if ((val = atoi(operator)) > 2 || val < 1)
			{
				printf("Struct NUM IS ILLEGAL ! ");
				free(temp);
				return -1;
			}
			if ((bcode = numToArrARE(val)) == NULL)
			{
				return -1;
			}
			pListInsert(&(*tail)->head, (*adress)++, bcode);
			(*IC)++;
		}
		else
		{
			printf("Struct NUM IS NOT INTEGER ! ");
			free(temp);
			return -1;
		}
		free(temp);
	}
	else if (kind == REG_ADDRESSING)
	{
		int val;
		/*an register addressing then parameter must be an legin num of register*/
		if ((val = findReg(operator)) != -1)
		{
			int *buf;
			buf = (int *)malloc(SIZE_OF_WORD * sizeof(int));
			if (buf == NULL)
			{
				printf("Allocation failure regBuff\n");
				exit(2);
			}
			memset(buf, 0, SIZE_OF_WORD * (sizeof(int)));
			if (regiterBuf(val, numOfOp, &buf))
			{
				return -1;
			}

			pListInsert(&(*tail)->head, (*adress)++, buf);
			(*IC)++;
		}
		else
		{
			printf("NOT LEGAL REGISTER ! ");
			return -1;
		}
		return 1;
	}
	return 1;
}

/*this function recieves addressing type ,operand , DOS (if the operand is destination or source) and returns 0 if legal , 1 if not */
int opAddresing(int addresing, int op, int DOS)
{
	int dest = 2, src = 1; /*TODO add printing if found an error also make sure of what number is returned if an error occure*/
	if (addresing == 0)
	{
		if (op <= 3 || op == 12)
		{
			if (op != 1 && DOS == src)
			{
				return 0;
			}
			else if ((op == 12 || op == 1) && DOS == dest)
				return 0;
			else
				return 1;
		}
	}
	else if (addresing == 1 || addresing == 2)
	{
		if (op <= 13)
		{
			if (DOS == dest)
			{
				return 0;
			}
			else if (op == 6 || op <= 3)
			{
				return 0;
			}
			else
				return 1;
		}
	}
	else if (addresing == 3)
	{
		if (op <= 13)
		{
			if (DOS == dest)
			{
				return 0;
			}
			else if (op <= 3)
			{
				return 0;
			}
			else
				return 1;
		}
	}
	return -1;
}
