# RISC-V Bare-Metal Labs — GD32VF103

Embedded systems coursework for the GD32VF103 RISC-V microcontroller (Sipeed
Longan Nano): RISC-V assembly exercises written against a course simulator, and
register-level C drivers written against real hardware.

The repository has two halves that target different environments and are not
built the same way. Roughly 21 assembly files and 20 C files, with 24 headers.

## The assembly half

`Assembly/`, `Laboration 1/` and `Laboration 2/` — plain RISC-V assembly, no
build system, no vendor library.

- **ALU operations** — `alu.S`: add, sub, and, or, xor, plus a NIM xor-sum demo.
- **Control flow** — `f2sr.S`: labelled if/then/else with `bne`/`beq`/`j`, and a
  countdown loop.
- **Stack and subroutines** — `f4stack.S`: `sw ra,0(sp)` prologue/epilogue,
  nested calls, callee-saved register spilling.
- **Checksum** — `f2cs.S`: byte sum with a two's-complement negation.
- **Packed BCD counter** — `f4bcd4dc.S`: a real four-digit BCD library with
  decimal adjust (`+6`, `+0x60`, `+0x600`) and wraparound at 10000.
- **LED matrix patterns** — `Robot.s`, `robot1.S`, `robot2.S`.
- **GPIO at register level** — `main.S` writes GPIOB (`0x40010C00`) CTR0/BOP/BC
  directly; `Laboration 1/` and `Laboration 2/` build pin init and bit
  manipulation on top.

Files in `Assembly/` that drive the LED matrix use a **course simulator calling
convention** — `li a0, 0x110` followed by `ecall` — not GD32 hardware
registers. They will not run on a Longan Nano unmodified.

Two files in `Assembly/` are **not RISC-V**, despite the directory:
`HelloWorld.asm` is MIPS (MARS/SPIM syntax) and `Adderatal.asm` is 16-bit x86
MASM for DOS.

## The C half

`Laboration 3/` through `Laboration 5/` — C applications and drivers using the
GD32VF103 firmware library and the Nuclei N200 ECLIC API, sharing a common
assembly driver package (`drivers.S`: `gpioi`, `gpiobo`, `keyinit`, `keyscan`,
`l88init`, `l88row`, `l88mem`).

| Lab | What it does |
| --- | --- |
| 3 · Level 1 | DAC output on PA4 and TIMER1 PWM on channels 0–3; keypad-controlled brightness |
| 3 · Level 2 | ADC-driven 8×8 LED game — potentiometer steering, score, game-over state |
| 4 · Level 1 | LCD battery-percentage display with custom glyphs, driven from the keypad |
| 4 · Level 2 | LCD drawing program — enter two points on the keypad, then draw rectangles, filled shapes, circles and lines from a ten-colour palette |
| 5 · Level 1 | ST7735-class LCD driver over SPI, plus ECLIC interrupt setup |
| 5 · Level 2 | Interrupt-driven USART with a 256-byte TX ring buffer, and ADC with calibration |

`drivers.S` is copied verbatim into five project directories rather than shared,
and `Laboration 5/Level 1` and `Level 2` are duplicates except for `main.c`.

## Building and running

**This repository does not build as checked out**, and that is worth stating
plainly rather than leaving you to discover it. There is no Makefile, no
`platformio.ini`, no linker script, and no startup code anywhere in the tree.

The C labs `#include <gd32vf103.h>` and `<gd32vf103_gpio.h>` and call into both
the GD32VF103 firmware library and the Nuclei ECLIC API. **None of that is
vendored here** — only the lab source is.

To build a lab you would need to:

1. Install PlatformIO with the `gd32v` platform (or `riscv-nuclei-elf-gcc` plus
   the Nuclei SDK directly).
2. Create a fresh project for the Sipeed Longan Nano.
3. Copy the lab's `src/` contents into the project's `src/` — note that `main.c`
   lives under `lib/LabN/src/` here, which is not where PlatformIO looks for an
   application entry point.
4. Move `drivers.S` into `src/` as well; it currently sits outside both.
5. Flash over DFU with `dfu-util`, or over JTAG.

The `Assembly/` files need a different environment again: **MARS or RARS** for
the simulator-based exercises and the MIPS file, and DOSBox with MASM for the
x86 file.

## Known gaps

- No build system, and the vendor SDK the C labs depend on is not included, so
  nothing here compiles without manual project setup.
- `drivers.S` is duplicated across five directories instead of shared, so a fix
  in one place does not reach the others.
- Two files in `Assembly/` target MIPS and x86 rather than RISC-V.
- Around 16 MB of the repository is demo video and screenshots
  (`Laboration 5/Level1.mp4`, `Level2.mp4`, `1.jpeg`, and a screenshot in
  `Laboration 2/`), which is most of its download size.

## License

[MIT](LICENSE)
