; Tests for stack group instructions
; Assumes data group works and STC.
.org 0x0
test_b:
	mvi b, 0xca
	mvi c, 0xfe
	push b
	mvi b, 0x00
	mvi c, 0x00
	pop b
	;Expect b, c = 0xcafe
test_d:
	mvi d, 0xbe
	mvi e, 0xef
	push d
	mvi d, 0x00
	mvi e, 0x00
	pop d
	;expect d, e, = 0xbeef
test_h:
	mvi h, 0xd0
	mvi l, 0x0d
	push h
	mvi h, 0x00
	mvi l, 0x00
	pop h
	;expect h, l = 0xd00d
test_p:
	mvi a, 0xcd
	push psw
	mvi a, 0x00
	stc
	pop psw
	;expect a = 0xcd, flags = 0x02
test_xthl:
	push d
	xthl
	pop d
	;expect h,l = 0xbeef, d,e = 0xd00d
test_sp:
	sphl
	;expect sp = 0xbeef
	lxi sp, 0xf00f
	;expect sp = 0xf00f
	inx sp
	;expect sp = 0xf010
	dcx sp
	;expect sp = 0xf00f
dec_push:
	;Test instruction decode
	push b
	push d
	push h
	push psw
dec_pop:
	pop b
	pop d
	pop h
	pop psw
dec_misc:
	xthl
	sphl
	hlt

