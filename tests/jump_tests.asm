; Tests for jump group instructions
; Assume data group and stack group works.
.org 0
	mvi e, 0xff
	push d
	pop psw
	;All flags set.
round1:
	jnc end
	nop
	jnz end
	nop
	jp end
	nop
	jpo end
	nop
end:
	hlt
