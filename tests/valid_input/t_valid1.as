MAIN:   add r6, K
LOOP:   prn #12
lea STR ,r6
inc r6
mov *r6,K
bne END
dec r1
macr hello
inc r3
sub r7,r5
endmacr
jsr LIST
jmp LOOP
END: stop
STR: .string "ab.cd"
LIST: .data 123,13,50,-1
.data -100,200
K: .data 31, 180