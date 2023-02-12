; file ps.as
.entry LOOP
.entry LENGTH
.extern L3
.extern W
MAIN:	mov	S1.1, W
	add	r2,STR
	macro trylose
	prn	#10
	sub r3 , r2
	not	K
	endmacro 	
LOOP:	jmp	W
	prn	#-5
	sub	r1, r4
	inc	K
	
	mov S1.2, r3
	trylose
	bne L3
END:	hlt
STR:	.string "abcdef"
LENGTH:	.data 6,-9,15
K:	.data 22
S1:	.struct 8, "ab"


