subrutin1:
        addi sp, sp, -4     # Make room on the stack...
        sw ra, 0(sp)        # ...for the return address!

        nop                 # Do something...
        call subrutin2      # ...with help from sub2!

        lw ra, 0(sp)        # Read back return address..
        addi sp, sp, 4      # ...and free used stack space!
        ret                 # Go home!

subrutin2:
        addi sp, sp, -8     # Make room on the stack...
        sw s0, 0(sp)        # Save s0
        sw s1, 4(sp)        # Save s1
        nop                 # Do something (that needs s0/s1)!
        lw s1, 4(sp)        # Retrive s1
        lw s0, 0(sp)        # Retrive s0
        addi sp, sp, 8      # Free used stack space!
        ret                 # Go home!

.globl main
main:   call subrutin1      # Do some work!

1:      J 1b                # (HALT)