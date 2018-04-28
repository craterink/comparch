.orig x1200
ADD R6 R6 #-2
STW R0, R6, #0
ADD R6 R6 #-2
STW R1, R6, #0
ADD R6 R6 #-2
STW R2, R6, #0
ADD R6 R6 #-2
STW R3, R6, #0

lea r0 dataloc
ldw r1 r0 #0
ldw r2 r1 #0
add r3 r2 #1
stw r3 r1 #0


LDW R3, R6, #0
ADD R6, R6, #2
LDW R2, R6, #0
ADD R6, R6, #2
LDW R1, R6, #0
ADD R6, R6, #2
LDW R0, R6, #0
ADD R6, R6, #2
rti

dataloc .fill x4000
.end
