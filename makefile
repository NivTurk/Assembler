assembler: assembler.o preassembler.o firstpass.o firstfunc.o base.o symbols.o functions.o hashtable.o linkedList.o secondpass.o
	gcc -ansi -Wall -pedantic preassembler.o firstpass.o firstfunc.o base.o symbols.o functions.o hashtable.o linkedList.o secondpass.o assembler.o -o assembler -lm

secondpass.o: secondpass.c secondpass.h
	gcc -c -ansi -Wall -pedantic secondpass.c -o secondpass.o	
firstpass.o: firstpass.c firstpass.h
	gcc -c -ansi -Wall -pedantic firstpass.c -o firstpass.o
firstfunc.o: firstfunc.c firstpass.h
	gcc -c -ansi -Wall -pedantic firstfunc.c -o firstfunc.o
base.o: base.c base.h
	gcc -c -ansi -Wall -pedantic -lm base.c -o base.o 
symbols.o: symbols.c symbols.h
	gcc -c -ansi -Wall -pedantic symbols.c -o symbols.o	
preassembler.o: preassembler.c functions.o preassembler.h 
	gcc -c -Wall -ansi -pedantic preassembler.c -o preassembler.o
functions.o: functions.c preassembler.h
	gcc -c -Wall -ansi -pedantic functions.c -o functions.o
hashtable.o: hashtable.c hashtable.h
	gcc -c -Wall -ansi -pedantic hashtable.c -o hashtable.o
linkedList.o: list.c linkedList.h
	gcc -c -Wall -ansi -pedantic list.c -o linkedList.o
assembler.o: assembler.c preassembler.h firstpass.h secondpass.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
clean:
	rm *.o *.am *.ent *.ext *.ob -f assembler
