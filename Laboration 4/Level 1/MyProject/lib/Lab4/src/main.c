#include "gd32vf103.h" // Registerdefinitioner för GD32VF103-mikrokontrollern
#include "drivers.h"   // Drivrutiner för diverse periferier
#include "lcd.h"       // Funktioner för LCD-hantering
#define MAX 16         // Key

int main(void)
{
  /*################ varibeler #############################*/
  int keyPressed;                                                               // Variabler för tangentbordshantering
  int keyMapping[MAX] = {15, 11, 0, 14, 12, 9, 8, 7, 13, 6, 5, 4, 10, 3, 2, 1}; // Tangentbordsmappning
  char specialChars[] = {126, 127, 128, 129, 130};                              // Batteriikoner definierade med ASCII-koder obs (större array)

  /*################ Initiera hårdvaran #############################*/
  keyinit();               // Initiera tangentbordet
  t5omsi();                // Startar timer
  colinit();               // Initierar kolumnhantering för tangentbord
  Lcd_SetType(LCD_NORMAL); // Sätt LCD-typ (t.ex. LCD_NORMAL)
  Lcd_Init();              // Initiera LCD-skärmen
  LCD_Clear(BLUE);         // Rensa LCD och fyll med blå bakgrund

  /*################ Nivå 1 #############################*/
  LCD_ShowStr(10, 10, "IBRAHIM VERSION", WHITE, TRANSPARENT); // copyright
  while (1)
  {
    if (t5expq())
    {
      l88row(colset());       // Uppdaterar tangentbordets rader
      keyPressed = keyscan(); // Skannar tangentbordet för att se om en knapp trycks
      if (keyPressed >= 0)
      {
        switch (keyMapping[keyPressed])
        {
        case 0:
          printPercentage("0%"); // printa ("0%")
          LCD_ShowChar(10 + 0 * 20, 50, specialChars[0], OPAQUE, WHITE);
          break;
        case 1:
          printPercentage("25%");
          LCD_ShowChar(10 + 1 * 20, 50, specialChars[1], OPAQUE, RED);
          break;
        case 2:
          printPercentage("50%");
          LCD_ShowChar(10 + 2 * 20, 50, specialChars[2], OPAQUE, YELLOW);
          break;
        case 3:
          printPercentage("75%");
          LCD_ShowChar(10 + 3 * 20, 50, specialChars[3], OPAQUE, WHITE);
          break;
        case 4:
          printPercentage("100%");
          LCD_ShowChar(10 + 4 * 20, 50, specialChars[4], OPAQUE, GREEN);
          break;
        default:
          LCD_Clear(BLUE); // utanför intervallet rensar skrämen.
          break;
        }
      }
    }
  }
  return 0; // Ofta inte nödvändigt i inbäddade system
}
