MAIN: 	add r3, LIST
LOOP: 	prn #48
		macr m_macr
		cmp r3, #-6
		bne END
		end_macr
		lea	STR, r6
		inc r6
		.entry HELLO
		mov *r6, K
		sub r1, r4
		dec HELLO
		m_macr
		dec	K
		jmp LOOP
END: 	stop
STR:		.string "abcd"
LIST:		.data 6,-9
		.data	 -100
K:		.data 31
