
assembler:assembler.o preprocess.o funcs.o firstpass.o secondpass.o
	gcc -g -ansi -Wall -pedantic assembler.o preprocess.o funcs.o firstpass.o secondpass.o -o assembler
assembler.o:assembler.c preprocess.h firstpass.h
	gcc -c -g -ansi -Wall -pedantic assembler.c -o assembler.o
preprocess.o:preprocess.c preprocess.h
	gcc -c -g -ansi -Wall -pedantic preprocess.c -o preprocess.o
funcs.o:funcs.c funcs.h
	gcc -c -g -ansi -Wall -pedantic funcs.c -o funcs.o
firstpass.o:firstpass.c firstpass.h 
	gcc -c -g -ansi -Wall -pedantic firstpass.c -o firstpass.o		
secondpass.o:secondpass.c secondpass.h 
	gcc -c -g -ansi -Wall -pedantic secondpass.c -o secondpass.o	
