.ORIG x3000
ADD R0 R1 #6
ADD R0 R0 R0
BRn label
BRp label
ADD R0 R7 #6
label JSR subr
XOR R0 R0 R0
XOR R0 R0 #1
done halt
A .fill x-1000
B .fill x2000
subr LEA R2 A
LDW R3 R2 #0
RSHFL R3 R3 #1
LSHF R3 R3 #2
RSHFA R3 R3 #2
STW R3 R2 #1
RET
.END
