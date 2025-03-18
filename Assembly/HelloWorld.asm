    .data                   # Dataavsnittet används för att lagra strängen
message: 
    .asciiz "Hello, World!\n"  # Strängen som ska skrivas ut

    .text                   # Textavsnittet innehåller instruktionerna
    .globl main             # Gör main till startpunkten
main:
    # Ladda adressen till strängen i register $a0
    la $a0, message         # Ladda adressen till strängen i $a0

    # Välj syscall för att skriva en sträng (kod 4)
    li $v0, 4               # Systemanrop: skriv ut sträng
    syscall                 # Utför systemanropet

    # Avsluta programmet
    li $v0, 10              # Systemanrop: avsluta program
    syscall                 # Utför avslutningen
