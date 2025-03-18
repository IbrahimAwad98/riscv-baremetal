    # 1:st part: addi & li...
    addi t0, zero, 0x007    # Load t0 with 7
    addi t1, zero, -1       # Load t1 with -1
    li t2, 0x1FFF           # Load t2 with 0x1FFF
    nop

    # 2:nd part: andi, ori & xori...
    li t0,7                 # Load t0 with 0b0111...
    andi t1, t0, 0xA        # ...andi with 0b1010...
    ori t2, t0, 0x8         # ...ori with  0b1000...
    xori t0, t0, 0xA        # ...xori with 0b1010.
    nop

    # 3:rd part: Nim with xor...
    li t0, 0x9              # Pile 1 in t0 (2)
    li t1, 0x5              # Pile 2 in t1
    li t2, 0x7              # Pile 3 in t2

    xor s0, t0, t1          # Compute NIM sum...
    xor s0, s0, t2          # ...if 0 you will lose!
    nop

    # 4:th part: add & sub...
    li t0, 0x9              # Set t0 to 0x9...
    li t1, 0x3              # ...and t1 to 0x3.

    add t2, t0, t1          # t2 = t0 + t1  (0xC)
    sub t2, t0, t1          # t2 = t0 - t1  (0x6)
    addi t2, t1, 1          # t2 = t1 + 1   (0x4)
    addi t2, t1,-1          # t2 = t1 - 1   (0x2)
    nop