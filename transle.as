
MAIN:	mov	S1.1,  LENGTH
	add	r2 ,STR
LOOP:	jmp END		
	macro m1	
		inc	K
		
		mov	S1.2, r6
	endmacro		
	prn	#-5

STR:	.string	"ab "cde"  f"	
LENGTH: .data	6 	,-9,	15	
	sub	r1 ,r4
	m1		
	bne	LOOP	
END:	hlt		

K:	.data	22	
S1:	.struct 8	,"acd "" lolb"	

