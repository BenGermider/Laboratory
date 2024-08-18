.entry LIST
.extern fn1
MAIN: add r3, WOOHOO
jsr fn1
LOOP: prn #48
 lea STR, r6
 inc #13
 mov *r6, L3
END: stop
STR: .string "abcd"
LIST: .data 6, -9
 .data -100
K: .data 31
.extern L3 