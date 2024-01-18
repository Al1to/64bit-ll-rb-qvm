entry _start

section .data
    test word 100

section .text
    _start:
        mov eax, 5
        lea ecx, &test
        cmp eax, [ecx]
        jl _label
        hlt

    _label:
        inc bl
        call _add_ebx
        inc rax
        hlt

    _add_ebx:
        add ebx, 234

        ret