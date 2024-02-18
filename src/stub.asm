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

		popx rax, rdi, rsi, rsp, rdx, rcx, r8, r9
		popfq


		jmp	0xFFFFFFFF
