MAIN:   add r3,LIST
LOOP:   prn #48
        macr M1
        cmp r3,#-6
bne END
    endmacr
lea STR ,r6
inc r6
mov *r6,K
sub r1,r4
M1
dec K
jmp LOOP
END: stop
STR: .string "ab.cd"
LIST: .data 123
.data -100
K: .data 31