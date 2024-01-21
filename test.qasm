entry _start

section .data
    test word 100

section .text
    _start:
        qreg q1[3]
        H q1[2]
        RST q1[2]
        ; meas ax, q1[2]

        hlt

