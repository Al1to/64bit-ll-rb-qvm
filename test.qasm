entry _start

section .data
      ; array start
    strt_and_size byte 03
    a byte 12
    b byte 37
    c byte 09
      ; array end

section .text
    _start:
        call _print_array

        hlt

    _print_array:
        push eax
        push ebx
        push dx

        lea ebx, &strt_and_size
        mov dl, [ebx]
        mov dh, 1
        add ebx, 1

        _strt_loop:
            mov eax, [ebx]
            out eax

            cmp dh, dl

            jge _end_loop

            inc dh
            add ebx, 1
            
            jmp _strt_loop

        _end_loop:
            pop dx
            pop ebx
            pop eax

            ret
