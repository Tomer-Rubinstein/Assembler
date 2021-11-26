MAIN: add $3 $5, $9
LOOP: ori $9, $5, $2
Next: move $20, $4, $9
bgt $4,2,END
movela  K
sw  $-1,4,$10
bne $35, $9, LOOP
call  ,vall
1END: stop
STR:  .asciz "aBcd" efg
    .dh 32800