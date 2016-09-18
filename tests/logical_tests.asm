; Tests for logical group instructions
;Test instruction decode - ana
.org 0
ana a
ana b
ana c
ana d
ana e
ana h
ana l
ana m
;" - ani
ani 0x00
;" - cmp
cmp a
cmp b
cmp c
cmp d
cmp e
cmp h
cmp l
cmp m
;" - cpi
cpi 0x00
;" - ora
ora a
ora b
ora c
ora d
ora e
ora h
ora l
ora m
;" - ori
ori 0x00
;" - rotate
ral
rar
rlc
rrc
;" - xra
xra a
xra b
xra c
xra d
xra e
xra h
xra l
xra m
;" - xri
xri 0x00
hlt
