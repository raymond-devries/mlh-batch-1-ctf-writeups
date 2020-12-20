section .text
global _start

_start:
	mov     ecx,4
	mov     eax,16
	add     ebx,4
	add     ecx,eax
	sub     ebx,eax
	xor	   ebx,ebx
	mov     eax,1
	int     0x80



