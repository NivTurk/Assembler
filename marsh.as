MAIN:	mov	S1.1 ,LENGTH
	add	r2,STR
LOOP:	jmp END
	 
	prn	#-5
	macro m3
		prn #127
		prn #-128
	endmacro
	macro m2
		inc	K
		
		mov	S1.2, r3
	endmacro 
	sub	r1,r4
	macro m1
		dec r1
		sub r1,r3
    	endmacro
	m3
 	m2
	m1
	bne	LOOP
END:	hlt
macro m4
		dec r7
		prn #30
	endmacro
STR:	.string	"abcdef"
	m4
LENGTH: .data	6,-9,15
K:	.data	22
S1:	.struct 8,"ab"
