MAIN: 	add r3, LIST
LOOP: 	prn #48
		macr m_macr 1234
		cmp r3, #-6
		bne END
		endmacr 4312
		lea	STR, r6
macr hey
inc r6
endmacr

		inc r6
		.entry HELLO
macr hey
dec r6
endmacr
		mov *r6, K
		sub r1, r4
macr mov
stop
endmacr
		dec HELLO
		m_macr
		dec	K
		jmp LOOP
END: 	stop
STR:		.string "abcd"
LIST:		.data 6,-9
		.data	 -100
K:		.data 31
