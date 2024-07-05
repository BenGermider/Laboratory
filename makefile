all: assembler

assembler: src/assembler.o src/common/hash_table.o src/common/utils.o src/pre_assembler.o
	gcc -ansi -Wall -pedantic src/common/hash_table.o src/assembler.o src/pre_assembler.o src/common/utils.o -o assembler

utils.o: src/common/utils.c include/common/utils.h
	gcc -ansi -Wall -pedantic -c src/common/utils.c -o src/common/utils.o

hash_table.o: src/common/hash_table.c include/common/hash_table.h
	gcc -ansi -Wall -pedantic -c src/common/hash_table.c -o src/common/hash_table.o

pre_assembler.o: src/pre_assembler.c include/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/pre_assembler.c -o src/pre_assembler.o

assembler.o: src/assembler.c include/common/hash_table.h include/common/utils.h include/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/assembler.c -o src/assembler.o

clear:
	find . -name "*.o" -type f -delete && rm -f assembler

