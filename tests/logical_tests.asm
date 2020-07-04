; Tests for logical group instructions
.org 0x0
dec_ana:
	;Test instruction decode - ana
	ana a
	ana b
	ana c
	ana d
	ana e
	ana h
	ana l
	ana m
dec_ani:
	;" - ani
	ani 0x00
dec_cmp:
	;" - cmp
	cmp a
	cmp b
	cmp c
	cmp d
	cmp e
	cmp h
	cmp l
	cmp m
dec_cpi:
	;" - cpi
	cpi 0x00
dec_ora:
	;" - ora
	ora a
	ora b
	ora c
	ora d
	ora e
	ora h
	ora l
	ora m
dec_ori:
	;" - ori
	ori 0x00
dec_rot:
	;" - rotate
	ral
	rar
	rlc
	rrc
dec_xra:
	;" - xra
	xra a
	xra b
	xra c
	xra d
	xra e
	xra h
	xra l
	xra m
dec_xri:
	;" - xri
	xri 0x00
hlt
