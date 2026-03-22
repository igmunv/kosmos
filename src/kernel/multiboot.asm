section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002)

section .text
global _start
extern kmain

_start:
    mov esp, stack_top   ; Настраиваем указатель стека
    call kmain           ; Прыгаем в твой Си-код
    cli                  ; Если kmain вернет управление - выключаем прерывания
.hang:
    hlt                  ; И замираем
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384           ; Резервируем 16 Кб под стек
stack_top:

