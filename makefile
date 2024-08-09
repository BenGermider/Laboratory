all: assembler

CODE = src/assembler/
COMMON = src/common/
COLLECTIONS = src/common/collections/

CODE_H = include/assembler/
COMMON_H = include/common/
COLLECTIONS_H = include/common/collections/

FLAGS = -ansi -Wall -pedantic

assembler: $(CODE)assembler.o $(COLLECTIONS)hash_table.o $(COMMON)utils.o $(CODE)pre_assembler.o $(COLLECTIONS)linked_list.o $(CODE)phase1_asm.o $(COMMON)library.o $(COLLECTIONS)sentence_list.o
	gcc $(FLAGS) $(COLLECTIONS)hash_table.o $(CODE)assembler.o $(CODE)pre_assembler.o $(COMMON)library.o $(CODE)phase1_asm.o $(COLLECTIONS)linked_list.o $(COMMON)utils.o $(COLLECTIONS)sentence_list.o -o assembler

utils.o: $(COMMON)utils.c $(COMMON_H)utils.h
	gcc $(FLAGS) -c $(COMMON)utils.c -o $(COMMON)utils.o

hash_table.o: $(COLLECTIONS)hash_table.c $(COLLECTIONS_H)hash_table.h
	gcc $(FLAGS) -c $(COLLECTIONS)hash_table.c -o $(COLLECTIONS)hash_table.o

library.o: $(COMMON)library.c $(COMMON_H)library.h
	gcc $(FLAGS) -c $(COMMON)library.c -o $(COMMON)library.o

linked_list.o: $(COLLECTIONS)linked_list.c $(COLLECTIONS_H)linked_list.h
	gcc $(FLAGS) -c $(COLLECTIONS)linked_list.c -o $(COLLECTIONS)linked_list.o

sentence_list.o: $(COLLECTIONS)sentence_list.c $(COLLECTIONS_H)sentence_list.h $(COMMON_H)data_types.h
	gcc $(FLAGS) -c $(COLLECTIONS)sentence_list.c -o $(COLLECTIONS)sentence_list.o

pre_assembler.o: $(CODE)pre_assembler.c $(CODE_H)pre_assembler.h
	gcc $(FLAGS) -c $(CODE)pre_assembler.c -o $(CODE)pre_assembler.o

assembler.o: $(CODE)assembler.c $(COLLECTIONS_H)hash_table.h $(COMMON_H)utils.h $(CODE_H)pre_assembler.h
	gcc $(FLAGS) -c $(CODE)assembler.c -o $(CODE)assembler.o

phase1_asm.o: $(CODE)phase1_asm.c $(CODE_H)phase1_asm.h $(COLLECTIONS_H)linked_list.h $(COMMON_H)consts.h $(COMMON_H)utils.h $(COMMON_H)data_types.h $(COMMON_H)library.h $(COMMON_H)sentence_list.h
	gcc $(FLAGS) -c $(CODE)phase1_asm.c -o $(CODE)phase1_asm.o

clear:
	find . -name "*.o" -type f -delete
	#find . -name "*.am" -type f -delete
	find . -name "*.ob" -type f -delete
	find . -name "*.ent" -type f -delete
	find . -name "*.ext" -type f -delete
	rm assembler
