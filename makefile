all: assembler

assembler: src/assembler/assembler.o src/common/hash_table.o src/common/utils.o src/assembler/pre_assembler.o
	gcc -ansi -Wall -pedantic src/common/hash_table.o src/assembler/assembler.o src/assembler/pre_assembler.o src/common/utils.o -o assembler

utils.o: src/common/utils.c include/common/utils.h
	gcc -ansi -Wall -pedantic -c src/common/utils.c -o src/common/utils.o

hash_table.o: src/common/hash_table.c include/common/hash_table.h
	gcc -ansi -Wall -pedantic -c src/common/hash_table.c -o src/common/hash_table.o

pre_assembler.o: src/assembler/pre_assembler.c include/assembler/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/assembler/pre_assembler.c -o src/assembler/pre_assembler.o

assembler.o: src/assembler/assembler.c include/common/hash_table.h include/common/utils.h include/assembler/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/assembler/assembler.c -o src/assembler/assembler.o

clear:
	find . -name "*.o" -type f -delete
	find . -name "*.am" -type f -delete
	find . -name "*.ob" -type f -delete
	find . -name "*.ent" -type f -delete
	find . -name "*.ext" -type f -delete
	rm assembler
