#################################################################
# BCD 4 Digit Counter Library				            1.1 AC	#
# Dependensis: None						                        #
#################################################################
.text
# Reset #########################################################
bcd4dc_reset:
	la	t0,bcd4dc	        # t0 points to counter...
	sh	zero,0(t0)	        # ...clear counter...
	ret			            # and go home!
	
# Tick ##########################################################
bcd4dc_tick:
	la	t0,bcd4dc	        # t0 points to counter...
	lhu t1,0(t0)	        # ...read counter...
	li	t2,0x9		        # ...overflow constant...

	addi t1, t1, 1	        # ...add 1
	andi t3, t1, 0xf	    # ...mask out one's
    ble	t3, t2, 1f  	    # ...if not one's overflow, done!
	addi t1, t1, 6	        # ...decimal adjust one's (+6)
	
	srli t3, t1, 4	        # ...retrieve the ten's
	andi t3, t3, 0xf	    # ...mask out ten's
	ble	t3, t2, 1f      	# ...if not ten´s overflow, done!
	addi t1, t1, 0x60	    # ...decimal adjust ten's (+6)	
		
	srli t3, t1, 8	        # ...retrieve the 100's
	andi t3, t3, 0xf	    # ...mask out 100's
	ble	t3, t2, 1f      	# ...if not 100´s overflow, done!
	addi t1, t1, 0x600	    # ...decimal adjust 100's (+6)	
	
	srli t3, t1, 12	        # ...retrieve the 1000's
	ble	t3, t2, 1f      	# ...if not 1000´ overflow, done!
	li t1, 0		        # ...wrap around!
                            #    (Intentional fall through)
1:
	sh	t1,0(t0)	        # ...save the new value...
	ret			            # and go home!
	
# Read ### A 4 digit BCD value returned in a0 ##################
bcd4dc_read:
	la	t0,bcd4dc	        # t0 points to counter...
	lhu	a0,0(t0)	        # ...read counter...
	ret			            # and go home!
.data
bcd4dc:	.half 0xFFFF
# End of BCD 4 Digit Counter Library ###########################

.text
.globl	main
main:	call	bcd4dc_reset	# UT: Check _reset

	    li	t6, 10002	
loop:	call bcd4dc_read	# UT: Check _read
        mv a3, a0           # Save retrived counter value in a3...
        li a0, 0x110        # ...Request ledmatrix row function...
        li a1, 0            # ...for Row #0...
        andi a2, a3, 0xF    # ...with 1:st digit...
        ecall
        li a1, 1            # ...for Row #1...
        srli a2, a3, 4      # ...with 2:nd digit...
        andi a2, a2, 0xF    # 
        ecall
        li a1, 2            # ...for Row #2...
        srli a2, a3, 8      # ...with 3:rd digit...
        andi a2, a2, 0xF    # 
        ecall
        li a1, 3            # ...for Row #3...
        srli a2, a3, 12     # ...with 4:th digit...
        andi a2, a2, 0xF    # (Should not be needed)
        ecall

	    call bcd4dc_tick	# UT: Check _counter
	    addi t6, t6, -1     # Decrement UT counter...
	    bnez t6, loop       # ...and continue if not done!
	
halt:	j halt	            # (HALT)
