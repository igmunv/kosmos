global _syscall

section .text

_syscall:
    push ebp
    mov ebp, esp

    mov eax, [ebp+8]    ; syscall number
    mov ebx, [ebp+12]   ; arg1
    mov ecx, [ebp+16]   ; arg2
    mov edx, [ebp+20]   ; arg3
    mov esi, [ebp+24]   ; arg4
    mov edi, [ebp+28]   ; arg5

    int 0x22

    pop ebp
    ret
