assembler: scanUtils.o outputFiles.o dataList.o assembler.o errorHandling.o essentials.o stringOperations.o structToBinary.o symbolTable.o second_pass.o first_pass.o memNode.o labelNode.o
	gcc -Wall -g -ggdb3 -pedantic -ansi scanUtils.o outputFiles.o dataList.o assembler.o errorHandling.o essentials.o stringOperations.o structToBinary.o symbolTable.o second_pass.o first_pass.o memNode.o labelNode.o -o assembler

assembler.o: assembler.c 
	gcc -c -g -Wall -pedantic -ansi assembler.c -o assembler.o

errorHandling.o: ./utils/errorHandling.c  
	gcc -c -g -Wall -pedantic -ansi ./utils/errorHandling.c -o errorHandling.o

essentials.o: ./utils/essentials.c
	gcc -c -g -Wall -pedantic -ansi ./utils/essentials.c -o essentials.o

stringOperations.o: ./utils/stringOperations.c  
	gcc -c -g -Wall -pedantic -ansi ./utils/stringOperations.c -o stringOperations.o

structToBinary.o: ./utils/structToBinary.c  
	gcc -c -g -Wall -pedantic -ansi ./utils/structToBinary.c -o structToBinary.o

symbolTable.o: ./utils/symbolTable.c  
	gcc -c -g -Wall -pedantic -ansi ./utils/symbolTable.c -o symbolTable.o

second_pass.o: second_pass.c  
	gcc -c -g -ansi -Wall -pedantic second_pass.c -o second_pass.o

first_pass.o: first_pass.c  
	gcc -c -g -ansi -Wall -pedantic first_pass.c -o first_pass.o

memNode.o: MemNode.c  
	gcc -c -g -ansi -Wall -pedantic MemNode.c -o memNode.o

labelNode.o: LabelNode.c  
	gcc -c -g -ansi -Wall -pedantic LabelNode.c -o labelNode.o

dataList.o: dataList.c
	gcc -c -g -ansi -Wall -pedantic dataList.c -o dataList.o

outputFiles.o: outputFiles.c
	gcc -c -g -ansi -Wall -pedantic outputFiles.c -o outputFiles.o

scanUtils.o: ./utils/scanUtils.c
	gcc -c -g -ansi -Wall -pedantic ./utils/scanUtils.c -o scanUtils.o
