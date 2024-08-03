all: assembler

assembler: src/assembler/assembler.o src/common/collections/hash_table.o src/common/utils.o src/assembler/pre_assembler.o src/assembler/phase1_asm.o src/common/library.o
	gcc -ansi -Wall -pedantic src/common/collections/hash_table.o src/assembler/assembler.o src/assembler/pre_assembler.o src/common/library.o src/assembler/phase1_asm.o src/common/utils.o -o assembler

utils.o: src/common/utils.c include/common/utils.h
	gcc -ansi -Wall -pedantic -c src/common/utils.c -o src/common/utils.o

hash_table.o: src/common/collections/hash_table.c include/common/collections/hash_table.h
	gcc -ansi -Wall -pedantic -c src/common/collections/hash_table.c -o src/common/collections/hash_table.o

library.o: src/common/library.c include/common/library.h
	gcc -ansi -Wall -pedantic -c src/common/library.c -o src/common/library.o

pre_assembler.o: src/assembler/pre_assembler.c include/assembler/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/assembler/pre_assembler.c -o src/assembler/pre_assembler.o

assembler.o: src/assembler/assembler.c include/common/collections/hash_table.h include/common/utils.h include/assembler/pre_assembler.h
	gcc -ansi -Wall -pedantic -c src/assembler/assembler.c -o src/assembler/assembler.o

phase1_asm.o: src/assembler/phase1_asm.c include/assembler/phase1_asm.h include/common/consts.h include/common/utils.h include/common/data_types.h include/common/library.h
	gcc -ansi -Wall -pedantic -c src/assembler/phase1_asm.c -o src/assembler/phase1_asm.o
clear:
	find . -name "*.o" -type f -delete
	find . -name "*.am" -type f -delete
	find . -name "*.ob" -type f -delete
	find . -name "*.ent" -type f -delete
	find . -name "*.ext" -type f -delete
	rm assembler
