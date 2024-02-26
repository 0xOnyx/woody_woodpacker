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

		section .text
_entry:
        pushfq
		pushx rax, rdi, rsi, rsp, rdx, rcx, r8, r9
        jmp _init_start

_start:
		mov rdi, 1
		pop rsi
		mov rdx, 14
		mov rax, 1
		syscall

        jmp _init_decrypt

_leave:
		popx rax, rdi, rsi, rsp, rdx, rcx, r8, r9
		popfq

		jmp	0xFFFFFFFF ; Replace with old entry point

_decrypt:
;init on stack sbuff with key at [rdi]
        pop rsi
        lea rdi, [rel _decrypt] ; Replace with text section start
        mov r11, 0xFFFFFF ; Replace with text section end


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
		jmp _leave

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

_init_start:
        call _start
	    woody db "....WOODY....", 0xA
_init_decrypt:
        call _decrypt
        key times 16 db 0x0
