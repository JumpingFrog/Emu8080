; Tests for jump group instructions
.org 0x0
rst0_entry:
	jmp start

.org 0x8
rst1_entry:
	ret

.org 0x10
rst2_entry:
	ret

.org 0x18
rst3_entry:
	ret

.org 0x20
rst4_entry:
	ret

.org 0x28
rst5_entry:
	ret

.org 0x30
rst6_entry:
	ret

;RST 7
.org 0x38
rst7_entry:
	ret

start:
	mvi e, 0xff
	push d
	pop psw
	;All flags set.
;Test instruction decode - jumps
j_a:
	jmp j_b
j_b:
	jc j_c
j_c:
	jnc end
j_d:
	jz j_e
j_e:
	jnz end
j_f:
	jp end
j_g:
	jm j_h
j_h:
	jpe j_i
j_i:
	jpo end
j_j:
;" - pchl
	lxi h, c_a
	pchl
;" - calls
c_a:
	call c_b
c_b:
	cc c_c
c_c:
	cnc end
c_d:
	cz c_e
c_e:
	cnz end
c_f:
	cp end
c_g:
	cm c_h
c_h:
	cpe c_i
c_i:
	cpo end
setup_returns:
; Setup stack for returns testing
	;ret target
	lxi h, r_i
	push h
	;rc target
	lxi h, r_h
	push h
	;rz target
	lxi h, r_e
	push h
	;rm target
	lxi h, r_c
	push h
	;rpe target
	lxi h, r_b
	push h
;" - returns
r_a:
	ret
r_b:
	rc
r_c:
	rnc
r_d:
	rz
r_e:
	rnz
r_f:
	rp
r_g:
	rm
r_h:
	rpe
r_i:
	rpo
;" - resets
;overwrite jmp at 0x00 with ret
rest_setup:
	lxi h, 0000h
	mvi m, 0c9h
rest_test:
	rst 0
	rst 1
	rst 2
	rst 3
	rst 4
	rst 5
	rst 6
	rst 7
end:
	hlt

