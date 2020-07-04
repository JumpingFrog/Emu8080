; Tests for arithmetic group instructions
; Assumes mvi is functional.
.org 0x0
start:
	mvi A, 0xAB
	mvi B, 0x11
	add B
	;should generate only sign
	mvi a, 0x0f
	add b
	;should generate aux carry
	mvi a, 0xff
	mvi b, 0x01
	add b
dec_add:
	;Test instruction decode - add
	add a
	add b
	add c
	add d
	add e
	add h
	add l
	add m
dec_adi:
	;" - adi
	adi 0x00
dec_adc:
	;" - adc
	adc a
	adc b
	adc c
	adc d
	adc e
	adc h
	adc l
	adc m
dec_aci:
	;" - aci
	aci 0x00
dec_dad:
	;" - dad
	dad b
	dad d
	dad h
	dad sp
dec_inr:
	;" - inr
	inr a
	inr b
	inr c
	inr d
	inr e
	inr h
	inr l
	inr m
dec_dcr:
	;" - dcr
	dcr a
	dcr b
	dcr c
	dcr d
	dcr e
	dcr h
	dcr l
	dcr m
dec_inx:
	;" - inx
	inx b
	inx d
	inx h
	inx sp
dec_dcx:
	;" - dcx
	dcx b
	dcx d
	dcx h
	dcx sp
dec_sub:
	;" - sub
	sub a
	sub b
	sub c
	sub d
	sub e
	sub h
	sub l
	sub m
dec_sui:
	;" - sui
	sui 0x00
dec_sbb:
	;" - sbb
	sbb a
	sbb b
	sbb c
	sbb d
	sbb e
	sbb h
	sbb l
	sbb m
dec_sbi:
	;" - sbi
	sbi 0x00
dec_daa:
	;" - daa
	daa
	hlt
