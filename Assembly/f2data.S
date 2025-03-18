.text
main:   la t0, number                   # t0 address to number...
        lw t1, 0(t0)                    # Load t1 with number
        lb t2, 4(t0)                    # Load t2 with 1:st byte of buf.
        lb t2, 5(t0)                    # Load t2 with 2:nd byte of buf.
        nop

.data
number: .word 0x12345678                # int  number=0x12345678;
buffer: .byte 0x55, 0xAA, 0x55, 0xAA    # char buffer[]={0x55...};
        .byte 0x55, 0xAA, 0x55, 0xAA
letter: .byte 0x41                      # char letter='A';
status: .byte 0x01                      # Boolean status=true;