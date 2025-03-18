#include "gd32vf103.h"
#include "drivers.h"
#include "lcd.h"
#define MAXCOLORS 10
#define MAX 16

int main(void)
{
  /*################ varibeler #############################*/
  int pressedKey;
  int inputCount = 0;                                                                                       // Räknar antalet inmatningar
  int digit;                                                                                                // Siffervärde från lookup-tabellen
  int row, column;                                                                                          // Temporära variabler för inmatade värden
  int pointARow, pointAColumn;                                                                              // Koordinater för punkt A
  int pointBRow, pointBColumn;                                                                              // Koordinater för punkt B
  int colorIndex = 0;                                                                                       // Index för vald färg
  int lookupTable[MAX] = {15, 11, 0, 14, 12, 9, 8, 7, 13, 6, 5, 4, 10, 3, 2, 1};                            // Lookup-tabell knappar
  int colorPalette[MAXCOLORS] = {BLACK, CYAN, WHITE, MAGENTA, GREEN, GRAY, BROWN, BLUE, LIGHTBLUE, YELLOW}; // Array för färger

  /*################ Initiera hårdvaran #############################*/
  t5omsi();                // Initiera timer5 med 1 kHz
  colinit();               // Initiera kolumnverktyg
  keyinit();               // Initiera tangentbordsverktyg
  Lcd_SetType(LCD_NORMAL); // Sätt LCD-typ (kan ändras till LCD_INVERTED vid behov)
  Lcd_Init();              // Initierar LCD-skärmen.
  LCD_Clear(RED);          // börja RED

  while (1)
  {
    LCD_WR_Queue(); // säker ställa att skrivningar utförs korrekt.

    if (t5expq()) // Timer 5
    {
      l88row(colset()); // Uppdatera 8x8 LED och tangentbord
      pressedKey = keyscan();

      if (pressedKey >= 0)
      {
        // Använd switch-case för att hantera knapptryckningar
        switch (lookupTable[pressedKey])
        {
        case 0:
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10: // Knappen A (värde 10)
          if (inputCount == 0 && pointARow != -1 && pointAColumn != -1)
          {
            // Om vi har en registrerad punkt A, ta bort den
            pointARow = -1; // flaggor
            pointAColumn = -1;
            LCD_Clear(RED); // Rensa skärmen för att visa att den är borttagen
          }
          else
          {
            digit = lookupTable[pressedKey];
            if (inputCount == 0)
            {
              row = digit; // Första inmatning
              inputCount++;
            }
            else if (inputCount == 1)
            {
              column = digit; // Andra inmatning
              pointARow = row;
              pointAColumn = column;
              inputCount = 0; // Inmatning klar
            }
            else
            {
              LCD_Clear(RED); // rensa
            }
          }
          break;
        case 14: // Rita rektangel * 16 bitar per rad och 8 per kolumn
          LCD_DrawRectangle((pointARow * 16), (pointAColumn * 8),
                            (pointBRow * 16), (pointBColumn * 8),
                            colorPalette[colorIndex]);
          LCD_Fill((pointARow * 16), (pointAColumn * 8),
                   (pointBRow * 16), (pointBColumn * 8),
                   colorPalette[colorIndex]);
          inputCount = 0;
          break;

        case 11: // Rita cirkel # (pythagoras sats man kan avnända sqr i math.h)
        {
          int radius = (pointBRow - pointARow) * (pointBRow - pointARow) + (pointBColumn - pointAColumn) * (pointBColumn - pointAColumn);
          Draw_Circle((pointARow * 16), (pointAColumn * 8), radius, colorPalette[colorIndex]);
          inputCount = 0;
        }
        break;
        case 13: // Sätt punkt B eller visa texten "IBB"
          if (inputCount == 0)
          {
            // Om inga koordinater har matats in, visa texten "IBBE"
            LCD_ShowStr(130, 60, "IBB", colorPalette[colorIndex], TRANSPARENT);
          }
          else if (inputCount == 2)
          {
            // Om vi har fått två inmatningar (rad och kolumn) registrera punkt B
            pointBRow = row;
            pointBColumn = column;
          }
          inputCount = 0; // Återställ räknaren oavsett vad som gjordes
          break;
        case 12: // Sätt färg C
          if (inputCount == 1)
          {
            colorIndex = row; // Använder den tidigare inmatade raden som färgindex
          }
          inputCount = 0;
          break;
        case 15: // Rita linje mellan punkt A och B D
          LCD_DrawLine((pointARow * 16), (pointAColumn * 8),
                       (pointBRow * 16), (pointBColumn * 8),
                       colorPalette[colorIndex]);
          inputCount = 0;
          break;
        default:
          // inget händer
          break;
        }
      }
    }
  }
  return 0;
}
