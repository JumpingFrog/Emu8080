; Tests for arithmetic group instructions
.org 0

mvi a, 0xab
mvi b, 0x11
add b
;should generate only sign
mvi a, 0x0f
add b
;should generate aux carry
mvi a, 0xff
mvi b, 0x01
add b
;should generate Z, C, AC
hlt
