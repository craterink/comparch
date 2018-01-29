		.orig x3000
		br start

base 	.fill x4000

		; init vars
start 	lea r1 base
		ldw r0 r1
		lea r1 base
		ldb r5 r0 #4
		and r2 r2 #0
		add r2 r2 #3

		; vars
		; r0 x4000	(src base addr)
		; r1 x4005 	(dest base addr)
		; r2 mask mask (initally 3)
		; r5 mask
		
		; shuffle into byte 0
		and r3 r2 r5
		rshfl r3 r3 #0 ;
		add r4 r1 r3
		ldb r6 r4 #0
		stb r6 r0 #0

		; shuffle into byte 1
		lshf r2 r2 #2
		and r3 r2 r5
		rshfl r3 r3 #2 ;
		add r4 r1 r3
		ldb r6 r4 #0
		stb r6 r0 #1

		; shuffle into byte 2
		lshf r2 r2 #2
		and r3 r2 r5
		rshfl r3 r3 #4;
		add r4 r1 r3
		ldb r6 r4 #0
		stb r6 r0 #2

		; shuffle into byte 3
		lshf r2 r2 #2
		and r3 r2 r5
		rshfl r3 r3 #6;
		add r4 r1 r3
		ldb r6 r4 #0
		stb r6 r0 #3

done	trap x25


