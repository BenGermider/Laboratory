all: hash

hash: file_handler.o linked_list.o hash.o
	gcc -ansi -Wall -pedantic file_handler.o hash.o linked_list.o -o hash

file_handler.o: file_handler.c file_handler.h
	gcc -ansi -Wall -pedantic -c file_handler.c -o file_handler.o

linked_list.o: linked_list.c linked_list.h
	gcc -ansi -Wall -pedantic -c linked_list.c -o linked_list.o

hash.o: hash.c file_handler.h
	gcc -ansi -Wall -pedantic -c hash.c -o hash.o

clear:
	rm *.o hash