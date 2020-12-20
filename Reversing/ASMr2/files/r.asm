section .text
global _start

_start:
	mov eax,-1
	mov ebx,3
	xor eax,ebx
	add ebx,ebx
	xor eax,ebx
	mov ebx,10
	and eax,ebx
	xor ebx,ebx
	cmp eax,ebx
	jz isZero
notZero:
	mov ebx,eax
	jmp exit
isZero:
	mov ebx,-1
exit:
	mov eax,1
	int 0x80
