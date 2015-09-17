.MODEL	SMALL
.186
.STACK	100h
.DATA
Message		DB 'Hello World', 13, 10, '$'
.CODE
Booth	PROC
	mov cx, 15               ; 1. Initialize the loop counter to 15
	mov bx, 6                ; 2. Place the multiplicand in the BX register (in this case, 6)
	mov ax, 3                ; 3. Place the multiplier in the AX register (in this case, 3)
	mov dx, 0                ; 4. Initialize DX to zero
	clc                      ; 5. Clear the carry flag
	; STEP 6: Check LSB of Multiplier and CF
	mov di, ax
	and di, 0001h            ; We're checking the least significant bit to see if it is 1 by ANDing the multiplier's LSB to with 1
	jz  LSB_0                ; Jump if the least significant bit was 0.
	; Since we didn't jump, LSB = 1
	jc  BOTH_EQUAL           ; If the carry flag is set, then both the LSB and the carry flag are = 1
	; Since we didn't jump, LSB = 1, CF = 0. Simply continue below
	;; TODO Step 8
	BOTH_EQUAL:
	;; TODO Step 10
	LSB_0:
		jnc BOTH_EQUAL       ; Jump if the least significant bit is 0 and the carry flag isn't set
		;; TODO Step 9
Booth	ENDP
	END	Hello
