; Tests for special group instructions
.org 0x0
start:
	;Test instruction decode
	cma
	cmc
	stc
	in 10
	out 10
	ei
	di
	nop
	hlt

