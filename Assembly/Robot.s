
    li a0, 0x110        # Kontrolladress för LED-matrisen
    li a1, 0xA          # Rad 10
    li a2, 0x1088       # Mönster för rad 10
    ecall               # Skicka till matrisen

    li a1, 0x9          # Rad 9
    li a2, 0x0070       # Mönster för rad 9
    ecall               # Skicka till matrisen

    li a1, 0x8          # Rad 8
    li a2, 0x0008       # Mönster för rad 8
    ecall               # Skicka till matrisen

    li a1, 0x7          # Rad 7
    li a2, 0x0000       # Mönster för rad 7
    ecall               # Skicka till matrisen

    li a1, 0x6          # Rad 6
    li a2, 0x0180       # Mönster för rad 6
    ecall               # Skicka till matrisen

    li a1, 0x5          # Rad 5
    li a2, 0x0420       # Mönster för rad 5
    ecall               # Skicka till matrisen

    li a1, 0x1          # Rad 1
    li a2, 0x3C3C       # Mönster för rad 1
    ecall               # Skicka till matrisen

    nop                 # Ingen ytterligare åtgärd
