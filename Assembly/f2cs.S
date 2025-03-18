#.section
.text
#.global main
main:   la t0, buf          # t0 points to data area

        lb t1, 0(t0)        # Get 1:st byte
        lb t2, 1(t0)        # Get 2:nd byte
        lb t3, 2(t0)        # Get 3:rd byte
        lb t4, 3(t0)        # Get 4:th byte

        add t5, t1, t2      # Compute sum...
        add t6, t3, t4  
        add t6, t6, t5
        sub t6, zero, t6    # ...and its 2-comp.

        sb t6, 4(t0)        # Save checksum!

1:      j 1b                # Done.

#.section
.data
buf:    .word 0x04030201    # Four byte msg buffer
        .byte 0x00          # and 1 byte checksum