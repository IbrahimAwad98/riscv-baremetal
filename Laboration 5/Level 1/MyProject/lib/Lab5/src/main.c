#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"
#include "usart.h"
#define EI 1 // enable
#define DI 0 // disable
#define MAX 16

int main(void)
{
  /*################ Variabeldeklaration #############################*/
  int pressedKey;                                                              // Håller tangentkoden från keyscan()
  int lookUpTbl[MAX] = {13, 15, 0, 14, 12, 9, 8, 7, 11, 6, 5, 4, 10, 3, 2, 1}; // Lookup-tabell
  int keyValue;                                                                // Det mappade värdet från lookup-tabellen
  int inputStep = 0;                                                           // Steg i inmatningsprocessen (0: tiotals, 1: ensiffrigt, 2: kombinera)
  int digitTens;                                                               // Tiotalsdel: multiplicerat med 16 för att placeras rätt
  int digitOnes;                                                               // Ensiffrig del
  int finalValue;                                                              // Slutgiltigt värde (kombination av tiotal och ensiffrigt)
  int lcdCursorX = 0;                                                          // Horisontell position på LCD där nästa tecken ska visas
  int lcdCursorY = 50;                                                         // längst ner i lcd display

  /*################ Initiera hårdvaran #############################*/
  t5omsi();                                                // Initiera timer5 med 1kHz
  colinit();                                               // Initiera kolumnverktygslåda
  keyinit();                                               // Initiera tangentbordsverktygslåda
  Lcd_SetType(LCD_NORMAL);                                 // Sätt LCD i normalt läge.
  Lcd_Init();                                              // Initiera LCD
  LCD_Clear(BLACK);                                        // Rensa LCD med svart bakgrund
  u0init(EI);                                              // Initiera USART0 med avbrott aktiverade
  eclic_global_interrupt_enable();                         // Aktivera globala avbrott
  LCD_ShowStr(10, 10, "LAB5: LEVEL1", WHITE, TRANSPARENT); // Visa headertexten på LCD (friviligt)

  /*################ NIVÅ 1 #############################*/
  while (1)
  {
    LCD_WR_Queue(); // Hantera LCD-kommandokö

    // Om data tas emot via USART0, visa tecknet på LCD
    if (usart_flag_get(USART0, USART_FLAG_RBNE))
    {
      LCD_ShowChar(lcdCursorX, lcdCursorY, usart_data_receive(USART0), TRANSPARENT, WHITE);
      // Om vi når slutet av skärmen, rensa den och starta om från vänster
      if (lcdCursorX > 160)
      {
        LCD_Clear(BLACK);
        lcdCursorX = 0;
      }
    }
    if (t5expq())
    {
      l88row(colset()); // Uppdatera 8x8 LED och tangentbord
      if ((pressedKey = keyscan()) >= 0)
      {
        keyValue = lookUpTbl[pressedKey];
        // Hantera inmatningsprocessen i tre steg:
        if (inputStep == 0)
        {
          // Steg 1: Ta emot tiotalsdelen (endast giltiga värden 2-7)
          if (keyValue >= 2 && keyValue <= 7)
          {
            digitTens = keyValue * 16; // Placera siffran på tiotalspositionen genom multiplikation med 16
          }
          inputStep++;
        }
        else if (inputStep == 1)
        {
          // Steg 2: Ta emot det ensiffriga värdet
          digitOnes = keyValue;
          inputStep++; // Gå vidare till kombineringssteget
        }
        else if (inputStep == 2)
        {
          // Steg 3: Kombinera tiotals- och ensiffriga delen
          finalValue = digitTens + digitOnes;      // t.ex. 4 och 1 => 41
          lcdCursorX += 9;                         // Uppdatera LCD-positionen för nästa tecken
          usart_data_transmit(USART0, finalValue); // Skicka det slutgiltiga värdet via USART0
          inputStep = 0;                           // Återställ inmatningssteget för nästa värde
        }
      }
    }
  }
  return 0; // Avsluta
}