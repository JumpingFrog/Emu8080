tty0tx equ 0x00
tty0rx equ 0x01
cr equ 0x0D
lf equ 0x0A

.org 0
start:
	lxi h, msg1
	call prints
	jmp ccp

ccp:
	call getchar
	cpi 'b' ;char is b, start boot process
	jz boot
	cpi 'd' ;char is d, dump some ram
	jz dump
	lxi h, msg2
	call prints
	jmp ccp

boot: hlt

dump: call gethex
	mov h, a
	call gethex
	mov l, a
	push h
	lxi h, msg3
	call prints
	pop h
	mvi c, 0x00 ;load loop counter in
dump_loop:
	call printhl
	mvi a, ':'
	out tty0tx
	mvi a, ' '
	out tty0tx
	mov a, m ;load from HL
	rrc ;upper 4 down
	rrc
	rrc
    rrc
	ani 0x0F ;upper 4
	call hex_makecha
	out tty0tx
	mov a, m ;reload HL [inefficent]
	ani 0x0F
	call hex_makecha
	out tty0tx
	mvi a, lf
	out tty0tx
	inx h
	inr c ;will gen a zero flag on overflow
	jnz dump_loop
	hlt

gethex: ;read hex address from console
	call getchar
	cpi 0x3A ;(less than this it's a number, more it's a digit
	jnc hexletter
	sui '0' ;it's a digit...
	rlc
	rlc
	rlc
	rlc
	jmp nexthex
hexletter:
	sui 0x57 ;'a'-0x0a
	rlc
	rlc
	rlc
	rlc
	jmp nexthex
nexthex:
	ani 0xF0 
	mov b, a ;save into a	
	call getchar ;and again
	cpi 0x3A
	jnc hexletter2
	sui '0'
	jmp endhex
hexletter2:
	sui 0x57 ;'a'-0x0a
	jmp endhex
endhex:
	ani 0x0F
	ora b
	ret	
	

getchar: ;reads in a char
	in tty0rx
	ori 0x00 ;get flags
	jz getchar ;loop if null
	out tty0tx
	ret

printhl: ;procedure to print HL (in hex) to tty0
	mov a, h
	rrc
	rrc
	rrc
	rrc
	ani 0x0F
	call hex_makecha
	out tty0tx
	mov a, h
	ani 0x0F
	call hex_makecha
	out tty0tx
	mov a, l ;do l
	rrc
	rrc
	rrc
	rrc
	ani 0x0F
	call hex_makecha
	out tty0tx
	mov a, l
	ani 0x0F
	call hex_makecha
	out tty0tx
	ret

hex_makecha: ;makes the lower 4 bits of acc a char
	cpi 0x0a
	jz hex_makecha_l
	jnc hex_makecha_l
	adi '0'
	ret
hex_makecha_l
	adi 0x57 ;'a'-0x0a
	ret

prints: ;Procedure to print a string to tty0
	mov a, m
	ori 0x00 ;get flags
	rz
	out tty0tx
	inx h
	jmp prints ;go round again

msg1: db '8080 Monitor ROM:',cr,lf,0x00
msg2: db 'Invalid choice (dnnnn or b)', cr, lf, 0x00
msg3: db cr, lf, 'Dumping 256:', cr, lf , 0x00

crlf: db cr, lf, 0x00
