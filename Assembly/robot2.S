    li a0, 0x110        # Mon. func. #110...
    li a3, 0x00         # (Invert/shift feature)
again:
    li a1, 0xB          # ...row #10,
    li a2, 0x0000       # ...with pattern  
    xor a2, a2, a3      # ...maybe inverted/shifted (sll)
    ecall               # ...fiat lux!
    li a1, 0xA          # ...row #10,
    li a2, 0x1088       # ...with pattern  
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x9          # ...row #9,
    li a2, 0x1088       # ...with pattern (389C)
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x8          # ...row #8,
    li a2, 0x1088       # ...with pattern
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x7          # ...row #7,
    li a2, 0x0080       # ...with pattern
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x6          # ...row #6,
    ecall               # ...fiat lux!
    li a1, 0x5          # ...row #5,
    li a2, 0x0180       # ...with pattern
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x4          # ...row #4,
    li a2, 0x0000       # ...with pattern  
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x3          # ...row #3,
    ecall               # ...fiat lux!
    li a1, 0x2          # ...row #2,
    li a2, 0x0420       # ...with pattern
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    li a1, 0x1          # ...row #1,
    li a2, 0x03C0       # ...with pattern
    xor a2, a2, a3      # ...maybe inverted
    ecall               # ...fiat lux!
    li a1, 0x0          # ...row #4,
    li a2, 0x0000       # ...with pattern  
    xor a2, a2, a3      # ...maybe inverted/shifted
    ecall               # ...fiat lux!
    xori a3, a3, -1     # ...toggle inv/shift mask! (1)
    j again
    nop


    
