.MODEL SMALL
.STACK 64

.DATA               ; DEKLARERA DATA
    DATA1 DB 10H    ; Variabel 1
    DATA2 DB 20H    ; Variabel 2
    SUM   DB ?      ; Summa (ej initialiserad)

.CODE
MAIN PROC FAR
         MOV AX, @DATA    ; Ladda datasegmentets början till AX
         MOV DS, AX       ; Flytta datasegmentets startadress till DS

    ; Hämtar och adderar värden
         MOV AL, DATA1    ; Ladda värdet från DATA1 till AL
         MOV BL, DATA2    ; Ladda värdet från DATA2 till BL
         ADD AL, BL       ; Addera AL och BL
         MOV SUM, AL      ; Spara resultatet i SUM

    ; DOS-avslutsfunktion
         MOV AH, 4CH      ; DOS-funktion för att avsluta programmet
         INT 21H          ; Anropa DOS-funktion

MAIN ENDP
END MAIN
