all: myset

myset: set.o myset.o
	gcc -ansi -Wall -pedantic set.o myset.o -o myset

set.o: set.c set.h
	gcc -ansi -Wall -pedantic -c set.c -o set.o

myset.o: myset.c myset.h set.h
	gcc -ansi -Wall -pedantic -c myset.c -o myset.o

clear:
	rm *.o myset
