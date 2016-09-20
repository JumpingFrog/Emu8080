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
a: jmp b
b: jc c
c: jnc d
d: jz e
e: jnz f
f: jp g
g: jm h
h: jpe i
i: jpo j
j:
;" - pchl
lxi h, k
pchl
;" - calls
k: call l
l: cc m
m: cnc n
n: cz o
o: cnz p
p: cp q
q: cm r
r: cpe s
s: cpo end
end: hlt

