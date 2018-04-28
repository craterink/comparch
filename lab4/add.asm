.orig x3000

; STEP 1: init x4000 to 1
lea r0 hex4000
ldw r1 r0 #0
and r4 r4 #0
add r4 r4 #1
stw r4 r1 #0

; STEP 2: sum bytes in xC000-xC013
lea r0 hexC000
ldw r2 r0 #0
lea r0 hexC014
ldw r3 r0 #0
lea r0 num20
ldw r0 r0 #0
and r5 r5 #0
loop ldb r7 r2 #0
ldb r7 r2 #0
add r5 r5 r7
add r2 r2 #1
add r0 r0 #-1
brz done
br loop

; STEP 3: store sum in xC014
done stw R5 R3 #0

; STEP 4a: test protection exception
;lea r0 hex0000
;ldw r0 r0 #0
;stw r5 r0 #0

; STEP 4b: test unaligned access exception
lea r0 hexC017
ldw r0 r0 #0
stw r5 r0 #0

; STEP 4c: test unused opcode exception
.fill xa000

hex4000 .fill x4000
hexC000 .fill xc000
hexC014 .fill xC014
hex0000 .fill x0000
hexC017 .fill xC017
num20 .fill #20
.end
