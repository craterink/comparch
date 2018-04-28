.orig x1000

; 1. pages 0-23 -> frames 0-23, marked valid and protected

.fill x0004
.fill x0204
.fill x0404
.fill x0604
.fill x0804
.fill x0a04
.fill x0c04
.fill x0e04
.fill x1004
.fill x1204
.fill x1404
.fill x1604
.fill x1804
.fill x1a04
.fill x1c04
.fill x1e04
.fill x2004
.fill x2204
.fill x2404
.fill x2604
.fill x2804
.fill x2a04
.fill x2c04
.fill x2e04

; 2. spot 24 -> frame 25, valid and unprotected
.fill x320c 

; FILL 71 spots with unprotected (x8)
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8

; 3. spot 96 -> frame 28. valid and unprotected
.fill x380c

; FILL 29 spots with unprotected (0x8)
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8
.fill x8

; 4. spot 126 -> frame 29, valid and unprotected
.fill x3a0c

.end ; last two spots 0, fine to end here
