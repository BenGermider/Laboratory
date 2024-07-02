all: hash

hash: file_handler.o linked_list.o hash_table.o hash.o utils.o
	gcc -ansi -Wall -pedantic file_handler.o utils.o hash.o hash_table.o linked_list.o -o hash

utils.o: utils.c utils.h
	gcc -ansi -Wall -pedantic -c utils.c -o utils.o

file_handler.o: file_handler.c file_handler.h utils.h
	gcc -ansi -Wall -pedantic -c file_handler.c -o file_handler.o

linked_list.o: linked_list.c linked_list.h
	gcc -ansi -Wall -pedantic -c linked_list.c -o linked_list.o

hash_table.o: hash_table.c hash_table.h file_handler.h utils.h
	gcc -ansi -Wall -pedantic -c hash_table.c -o hash_table.o

hash.o: hash.c file_handler.h linked_list.h hash_table.h
	gcc -ansi -Wall -pedantic -c hash.c -o hash.o

clear:
	rm *.o hash