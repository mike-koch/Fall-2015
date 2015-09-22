.MODEL	SMALL
.186
.STACK	100h
.DATA
MultiplicandPrompt		DB 'Enter Multiplicand: ', '$'
MultiplierPrompt		DB 10, 13, 'Enter Multiplier: ', '$'
ResultText				DB 10, 13, 'Result: ', '$'
.CODE
includelib UTIL
extrn getdec:near
extrn putdec:near
Booth	PROC
	mov cx, 16
	;mov dx, MultiplicandPrompt
	;mov ah, 9
	;int 0x21
	call getdec
	mov bx, ax
	call getdec
	mov dx, 0
	clc
	; STEP 6: Check LSB of Multiplier and CF. Depending on the values of the LSB and CF, different operations will occur.
	PROCEDURE:
		mov di, ax
		; I'm rotating the carry bit into SI before the AND operation, as the AND operation will always clear the carry flag. I then rotate the carry bit back out of SI.
		rcl si, 1
		and di, 0001h
		rcr si, 1
		jz  LSB_0
		jc  BOTH_EQUAL
		; Step 8: Subtract BX from DX
		sub dx, bx
		BOTH_EQUAL:
			; Step 10: Shift DX right by 1 position, then shift AX right 1 through the carry bit.
			sar dx, 1
			rcr ax, 1
			dec cx
			jcxz FINISH
			jmp PROCEDURE
		LSB_0:
			jnc BOTH_EQUAL
			; Step 9: Add BX to DX
			add dx, bx
			jmp BOTH_EQUAL
	FINISH:
		call putdec
		mov ax, 4c00h
		int 21h
Booth	ENDP
	END	Booth
