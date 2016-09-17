; Tests for arithmetic group instructions
; Assumes mvi is functional.
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

;Test instruction decode - add
add a
add b
add c
add d
add e
add h
add l
add m
;" - adi
adi 0x00
;" - adc
adc a
adc b
adc c
adc d
adc e
adc h
adc l
adc m
;" - aci
aci 0x00
;" - dad
dad b
dad d
dad h
dad sp
;" - inr
inr a
inr b
inr c
inr d
inr e
inr h
inr l
inr m
;" - dcr
dcr a
dcr b
dcr c
dcr d
dcr e
dcr h
dcr l
dcr m
;" - inx
inx b
inx d
inx h
inx sp
;" - dcx
dcx b
dcx d
dcx h
dcx sp
hlt
