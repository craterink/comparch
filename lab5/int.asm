.orig x1200
ADD R6 R6 #-2
STW R0, R6, #0
ADD R6 R6 #-2
STW R1, R6, #0
ADD R6 R6 #-2
STW R2, R6, #0
ADD R6 R6 #-2
STW R3, R6, #0

lea r0 ptloc
ldw r1 r0 #0
lea r0 ptsize
ldw r2 r0 #0
lea r0 ptemask
ldw r3 r0 #0

loop ldw r0 r1 #0
and r0 r0 r3
stw r0 r1 #0
add r1 r1 #2
add r2 r2 #-1
brp loop

LDW R3, R6, #0
ADD R6, R6, #2
LDW R2, R6, #0
ADD R6, R6, #2
LDW R1, R6, #0
ADD R6, R6, #2
LDW R0, R6, #0
ADD R6, R6, #2

rti

ptloc .fill x1000
ptsize .fill #32
ptemask .fill xfffe

.end
