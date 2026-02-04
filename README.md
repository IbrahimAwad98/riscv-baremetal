# RISC-V Assembly Programming

En samling laborationer och exempel i RISC-V assembler för inbyggda system. Projektet innehåller grundläggande assemblerprogram, GPIO-hantering, hårdvaruinterface och praktiska tillämpningar för mikrokontroller (GD32VF103).

## Beskrivning

Detta är ett utbildningsprojekt som demonstrerar RISC-V assemblerprogrammering från grunderna till avancerade hårdvaruinterface. Projektet är strukturerat som laborationer som bygger på varandra och täcker allt från aritmetiska operationer till avbrottshantering och periferiåtkomst.

## Funktioner

- Grundläggande ALU-operationer (addition, subtraktion, logiska operationer)
- Kontrollflöden (if-else, loopar, hopp)
- GPIO-hantering (pininitiering, bit operations)
- DAC (Digital-to-Analog Converter) styrning
- PWM (Pulse Width Modulation) för motorstyrning
- Tangentbordsavläsning och LED-matrisstyrning
- BCD (Binary-Coded Decimal) räknare
- Stack-hantering och subrutiner
- Checksum-beräkning
- USART seriell kommunikation
- ADC (Analog-to-Digital Converter)
- LCD-displayhantering
- Avbrottshantering (ECLIC)

## Teknikstack

- Språk: RISC-V Assembly (.S/.s), C
- Arkitektur: RISC-V 32-bit
- Hårdvara: GD32VF103 mikrokontroller (Longan Nano)
- Perifer: GPIO, DAC, PWM, TIMER, USART, ADC, SPI
- Development: Kommentarer på svenska och engelska

## Projektstruktur

```
RISC-V-/
├── Assembly/               # Grundläggande assemblerexempel
│   ├── alu.S              # ALU-operationer (add, sub, and, or, xor)
│   ├── f2sr.S             # Selektions- och repetitionsstrukturer
│   ├── Swap.s             # XOR-baserad variabelväxling
│   ├── f2cs.S             # Checksum-beräkning
│   ├── f2data.S           # Datahantering (.data-sektion)
│   ├── f4stack.S          # Stackoperationer och subrutiner
│   ├── f4bcd4dc.S         # 4-siffrig BCD-räknare
│   ├── Robot.s            # LED-matris robotmönster
│   └── main.S             # GPIO och RCU grundfunktioner
├── Laboration 1/          # GPIO-grundfunktioner
│   ├── level1.S           # GPIO initiering (gpioi)
│   └── level2.S           # GPIO bit operations (gpiobo)
├── Laboration 2/          # GPIO-tillämpningar
│   ├── Level1.S           # Förbättrad GPIO bitmanipulation
│   └── Level2.S           # Utökad GPIO-funktionalitet
├── Laboration 3/          # DAC och PWM
│   ├── Level 1/
│   │   └── lib/Lab3/src/
│   │       ├── dac.c      # DAC-initiering och styrning
│   │       ├── pwm.h      # PWM header
│   │       └── main.c     # Tangentbordsbaserad ljusstyrkereglering
│   └── Level 2/
│       └── projects/MyProject/
│           └── drivers.S  # Assembly GPIO-drivrutiner
├── Laboration 4/          # (Struktur tillgänglig)
├── Laboration 5/          # USART, LCD och ADC
│   ├── Level 1/
│   │   └── lib/Lab5/src/
│   │       ├── lcd.c      # LCD-drivrutin med SPI
│   │       └── eclicw.c   # ECLIC avbrottshantering
│   └── Level 2/
│       └── lib/Lab5/src/
│           ├���─ usart.c    # USART kommunikation med buffring
│           └── include/adc.h # ADC header
```
