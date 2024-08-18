CC := gcc
CFLAGS := -ansi -Wall -pedantic

CODE := src/assembler/
COMMON := src/common/
COLLECTIONS := src/common/collections/
OPERANDS := src/common/operands/

CODE_H := include/assembler/
COMMON_H := include/common/
COLLECTIONS_H := include/common/collections/
OPERANDS_H := include/common/operands/

SRCS := $(wildcard $(CODE)*.c $(COMMON)*.c $(COLLECTIONS)*.c $(OPERANDS)*.c)
OBJS := $(SRCS:.c=.o)

.PHONY: all clean

all: assembler

assembler: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CODE)%.o: $(CODE)%.c $(CODE_H)%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(COMMON)%.o: $(COMMON)%.c $(COMMON_H)%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(COLLECTIONS)%.o: $(COLLECTIONS)%.c $(COLLECTIONS_H)%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OPERANDS)%.o: $(OPERANDS)%.c $(OPERANDS_H)%.h
	$(CC) $(CFLAGS) -c $< -o $@

clear:
	find . -name "*.o" -type f -delete
	find . -name "*.am" -type f -delete
	find . -name "*.ob" -type f -delete
	find . -name "*.ent" -type f -delete
	find . -name "*.ext" -type f -delete
	rm assembler