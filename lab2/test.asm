    .orig x3000

     AND R0, R0, #0 ; R0 = 0
     ADD R0, R0, #1 ; R0 = 1
     AND R0, R0, #0 ; R0 = 0
     ADD R0, R0, #-1; R0 = -1
     AND R0, R0, #-1; R0 = -1
     AND R0, R0, #0 ; R0 = 0

     LEA R0, func   ; R0 ->func
     JMP R0
     
back LEA R0, val
     STB R2, R0, #2
     STB R3, R0, #3
     LEA R1, val1
     LDB R4, R1, #0 ; R4 = 0xAA
     LDB R5, R1, #1 ; R5 = 0xBB

     TRAP x25

func LEA R1, val    ; R1 = ->val
     LDB R2, R1, #0 ; R2 = 0xAA
     LDB R3, R1, #1 ; R3 = 0xBB
     LEA R1, func   ; R1 = ->func
     LEA R7, back
     RET            ; PC = R7

val  .fill xBBAA
val1 .fill xDDCC

     .end

