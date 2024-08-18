.extern EXT8TEST
    .entry ENT8TEST

ENT8TEST: sub *r1 ,*r6

macr MACRO_TESTING
inc x
dec x
endmacr

Pk300:   mov *r3, LENGTH
LOOP:   jmp L1

macr BEN_TESTING
bne END
prn #-5
bne EXT8TEST

endmacr
 .extern LENGTH

L1:     inc K
bne LOOP

BEN_TESTING
END:    stop

K:      .data -700,-800,-900,-111,-901,                        6

x: .data 104
