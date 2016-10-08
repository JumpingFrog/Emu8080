; Tests for data group instructions
.org 0

;test mvir
mvir:
	mvi a, 0x01
	mvi b, 0x02
	mvi c, 0x04
	mvi d, 0x08
	mvi e, 0x10
	mvi h, 0x0ff
	mvi l, 0x0fe
mvim:
	mvi m, 0xbe
movrm:
	mov a, m
	;expect a = 0xbe
movrr:
	mov a, a
	mov b, a
	mov c, a
	mov d, a
	mov e, a
	mov h, a
	mov l, a
	;expect all = 0xbe
	mvi b, 0x12
	mov a, b
	mov b, b
	mov c, b
	mov d, b
	mov e, b
	mov h, b
	mov l, b
	;expect all = 0x12
	mvi c, 0x34
	mov a, c
	mov b, c
	mov c, c
	mov d, c
	mov e, c
	mov h, c
	mov l, c
	;expect all = 0x34
	mvi d, 0x56
	mov a, d
	mov b, d
	mov c, d
	mov d, d
	mov e, d
	mov h, d
	mov l, d
	;expect all = 0x56
	mvi e, 0x78
	mov a, e
	mov b, e
	mov c, e
	mov d, e
	mov e, e
	mov h, e
	mov l, e
	;expect all = 0x78
	mvi h, 0x9a
	mov a, h
	mov b, h
	mov c, h
	mov d, h
	mov e, h
	mov h, h
	mov l, h
	;expect all = 0x9a
	mvi l, 0xbc
	mov a, l
	mov b, l
	mov c, l
	mov d, l
	mov e, l
	mov h, l
	mov l, l
	;expect all = 0xbc
lxi:
	lxi b, 0xdead
	lxi d, 0xbeef
	lxi h, 0xcafe
stax:
	mvi a, 0xd0
	stax b
	mvi a, 0x0d
	stax d
	mov h, b
	mov l, c
	mov b, m
	;expect b = 0xd0
	mov h, d
	mov l, e
	mov c, m
	;expect c = 0x0d
ldax:
	lxi b, 0xbeef
	lxi d, 0xdead
	ldax d
	;expect a = 0xd0
	ldax b
	;expect a= 0x0d
sta:
	sta 0xd00d
	mvi h, 0xd0
	mvi l, 0x0d
	mov b, m
	;expect b = 0x0d
lda:
	lda 0xfffe
	;expect a = 0xbe
shld:
	shld 0xabcd ;[0xabcd] = 0xd00d
	mvi h, 0xab
	mvi l, 0xcd
	mov b, m
	;expect b = 0x0d
	mvi l, 0xce
	mov c, m
	;expect c = 0xd0
lhld:
	lhld 0xabcd
	;expect h,l = 0xd00d
xchg:
	lxi d, 0xcafe
	mvi h, 0xbe
	mvi l, 0xef
	xchg
;Test instruction decode - mvi
	mvi a, 0
	mvi b, 0
	mvi c, 0
	mvi d, 0
	mvi e, 0
	mvi h, 0
	mvi l, 0
	mvi m, 0
;" - mov
	mov a, a
	mov a, b
	mov a, c
	mov a, d
	mov a, e
	mov a, h
	mov a, l
	mov a, m

	mov b, a
	mov b, b
	mov b, c
	mov b, d
	mov b, e
	mov b, h
	mov b, l
	mov b, m

	mov c, a
	mov c, b
	mov c, c
	mov c, d
	mov c, e
	mov c, h
	mov c, l
	mov c, m

	mov d, a
	mov d, b
	mov d, c
	mov d, d
	mov d, e
	mov d, h
	mov d, l
	mov d, m

	mov e, a
	mov e, b
	mov e, c
	mov e, d
	mov e, e
	mov e, h
	mov e, l
	mov e, m

	mov h, a
	mov h, b
	mov h, c
	mov h, d
	mov h, e
	mov h, h
	mov h, l
	mov h, m

	mov m, a
	mov m, b
	mov m, c
	mov m, d
	mov m, e
	mov m, h
	mov m, l
;" - lxi
	lxi b, 0
	lxi d, 0
	lxi h, 0
	lxi sp, 0
;" - stax
	stax b
	stax d
;" - ldax
	ldax b
	ldax d
;" - sta, lda
	sta 0
	lda 0
;" - shld, lhld
	shld 0
	lhld 0
;" - xchg
	xchg
hlt
