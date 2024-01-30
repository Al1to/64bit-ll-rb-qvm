entry _start

section .text

    _start:
        qreg q1[1]
        qreg q2[1]

        H q1[0]
        H q2[0]

        meas ah, q1[0]
        meas al, q2[0]

        cmp ah, al
        jl _al
        je _e
        jg _ah

        _al:
            ; if ah < al out 0
            mov bx, 0
            out bx
            hlt

        _e:
            ; if ah == al out 1
            mov bx, 1
            out bx
            hlt

        _ah:
            ; if ah > al out 2
            mov bx, 2
            out bx
            hlt
