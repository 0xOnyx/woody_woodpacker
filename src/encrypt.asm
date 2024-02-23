BITS 64

%macro pushx 1-*
 %rep %0
   push %1
   %rotate 1
 %endrep
%endmacro

%macro popx 1-*
  %rep %0
    %rotate -1
    pop %1
  %endrep
%endmacro

		global _start

		section .data

		key db "1234567890123456"
		code db "my very secret message is Hello World!"

		section .text
_start:
		pushfq
		pushx rax, rdi, rsi, rsp, rdx, rcx, r8, r9

		push 65
		mov rdi, 1
		mov rsi, rsp
		mov rdx, 1
		mov rax, 1
		syscall
		pop rax

		mov rsi, key
		mov rdi, code
		call decrypt

		popx rax, rdi, rsi, rsp, rdx, rcx, r8, r9
		popfq



		jmp	0xFFFFFFFF

;decrypt code at [rdi] with len r11 with key at [rsi] and len 16 bytes
decrypt:
;init on stack sbuff with key at [rdi]
		push rbp,
		mov rbp, rsp
		sub rsp, 256
		mov rax, rsp
		xor rcx, rcx
loop_init:
		mov [rax+rcx], cx
		add rcx, 1
		cmp rcx, 256
		jl loop_init
		
		xor rcx, rcx
		xor rdx, rdx
loop_key_init:
		add rdx, [rax+rcx]
		mov r8, rcx
		and r8, 15 ; mod keylength (16)
		add rdx, [rsi+r8]
		and rdx, 255 ; mod 256
		mov byte r8b, [rax+rcx] ; swap values of s[i] and s[j]
		mov byte r9b, [rax+rdx]
		mov [rax+rcx], r9b
		mov [rax+rdx], r8b
		add rcx, 1
		cmp rcx, 256
		jl loop_key_init

		xor rcx, rcx
		xor rdx, rdx
		xor r10, r10
loop_decrypt:
		call get_byte
		mov r8b, [rdi+r10]
		xor r9b, r8b
		mov [rdi+r10], r9b

		add r10, 1
		cmp r10, r11 ; TODO replace num by code end
		jl loop_decrypt

		leave
		ret

;get next byte from sbuff at [rax] and return in r9b
get_byte:
	add rcx, 1 ; i = (i + 1) % 256
	and rcx, 255
	add rdx, [rax+rcx] ; j = (j + sbuff[i]) % 256
	and rdx, 255
	mov byte r8b, [rax+rcx] ; swap values of s[i] and s[j]
	mov byte r9b, [rax+rdx]
	mov [rax+rcx], r9b
	mov [rax+rdx], r8b
	mov r8, [rax+rcx] ; t = (sbuff[i] + sbuff[j]) % 256
	add r8, [rax+rdx]
	and r8, 255
	mov byte r9b, [rax+r8] ; return sbuff[t]
	ret
	