; t_valid2.as
.extern pTsD
    .entry FIRST
    .entry second2

; HELLO?

ALO:   mov #5, *r0
jsr second2
jsr pTsD

.data 1, -2, 3, -4
.string "20465b"

FIRST: mov #19, r1
inc r7
        jsr FIRST

second2: cmp #1, r1
        bne LAST
        add #99, *r2
; comment test
        jmp FIRST

second2: .string "Merry Christmas"
    .data 19

LAST: prn r2
stop