# Project

RAM storages 0-4095. Every room in the RAM is 15bit.
Usable only from storage 100.

Registers: r0 - r7. Size 15 bits, 0-14.
Supporting only integers and chars.

In the field of opcode we write the number of the operation
For example mov is 0, them opcode for mov will be 0000.
Group 1: mov cmp add sub lea (2 operands)

Group 2: not clr inc dec jmp bne red prn jsr (1 operand)

Group 3: rts stop (0 operands)

## Assembly Language:

#### Macros:

macr *macro name*
content
endmacr

then in file whenever *macro name* is shown, must be replaced with content.
Name of a command must not be a macro name (validation must be done)

Must create a file with content of macros instead of their names.

Run over the black-list of the words and make sure the macro name
doesn't appear in the black list.

#### Sentences:

Seperated by new-line ('\n')
* Empty - Whitespaces only (isspace).
* Comment - Starts with semicolon, assembler must ignore.


### Required Utilities

* Hash-map (Save macros efficiently)
* Queues (Implementable (?))
* 