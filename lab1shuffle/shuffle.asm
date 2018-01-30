	.orig x3000
	br start

base 	.fill x4000

	; init vars
start 	lea r1 base
	ldw r0 r1
	ldb r5 r0 #4
	and r2 r2 #0
	add r2 r2 #3
        add r1 r0 #5
        and r7 r7 #0
        add r7 r7 #3

	; vars
	; r0 x4000	(src base addr)
	; r1 x4005 	(dest base addr)
	; r2 mask mask  (0x03, bottom two bits)
	; r5 mask
        ; r7 loop counter

	; shuffle into byte 0
loop	and r3 r2 r5
	add r4 r0 r3
	ldb r6 r4 #0
	stb r6 r1 #0
        
        rshfl r5 r5 #2
        add r1 r1 #1
        add r7 r7 #-1
        brn loop

done    trap x25

