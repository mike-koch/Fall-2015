.MODEL	SMALL
.186
.STACK	100h
.DATA
Message		DB 'Some message', 13, 10, '$'
.CODE
includelib UTIL
extrn getdec:near
extrn putdec:near
Booth	PROC
	mov cx, 15               ; 1. Initialize the loop counter to 15
	call getdec              ; Get a decimal value for the multiplicand
	mov bx, ax               ; Move that value into ax
	call getdec              ; Get a decimal value for the multiplier
	mov dx, 0                ; 4. Initialize DX to zero
	clc                      ; 5. Clear the carry flag
	; STEP 6: Check LSB of Multiplier and CF
	PROCEDURE:
		mov di, ax
		and di, 0001h            ; We're checking the least significant bit to see if it is 1 by ANDing the multiplier's LSB to with 1
		jz  LSB_0                ; Jump if the least significant bit was 0.
		; Since we didn't jump, LSB = 1
		jc  BOTH_EQUAL           ; If the carry flag is set, then both the LSB and the carry flag are = 1
		; Since we didn't jump, LSB = 1, CF = 0. Simply continue below
		sub bx, dx               ; Subtract the multiplicand from the high word of DX:AX
		BOTH_EQUAL:
			sar dx, 1            ; Perform a 'shift arithmetic right' 1 to shift high word into DX into the low word of partial product DX:AX
			rcr ax, 1            ; Rotate through carry the low bit 1 place to make it the new carry flag value
			dec cx               ; Decrement the loop counter
			jcxz FINISH          ; If cx is 0, let's exit the program
			jmp PROCEDURE        ; Otherwise we'll perform another iteration
		LSB_0:
			jnc BOTH_EQUAL       ; Jump if the least significant bit is 0 and the carry flag isn't set
			add bx, dx           ; Add the multiplicand from the high word of DX:AX
			jmp BOTH_EQUAL
	FINISH:
		call putdec
		mov ax, 4c00h
		int 21h
Booth	ENDP
	END	Booth
