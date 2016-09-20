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
;Test instruction decode - jumps
j_a: jmp j_b
j_b: jc j_c
j_c: jnc j_d
j_d: jz j_e
j_e: jnz j_f
j_f: jp j_g
j_g: jm j_h
j_h: jpe j_i
j_i: jpo j_j
j_j:
;" - pchl
lxi h, c_a
pchl
;" - calls
c_a: call c_b
c_b: cc c_c
c_c: cnc c_d
c_d: cz c_e
c_e: cnz c_f
c_f: cp c_g
c_g: cm c_h
c_h: cpe c_i
c_i: cpo c_j
c_j:
;" - returns setup stack
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
r_a: ret
r_b: rc
r_c: rnc
r_d: rz
r_e: rnz
r_f: rp
r_g: rm
r_h: rpe
r_i: rpo
end: hlt

