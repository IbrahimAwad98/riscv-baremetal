#include "drivers.h"
#include "dac.h"
#include "pwm.h"
#include <stdio.h>

#define MAX 16
#define BUFFER 4

int main(void)
{
  // Variabler för tangentbordshantering och logik
  int keyPressed, idleCounter = 0;

  // Tangentbordsmappning (översätter tangentkoder till specifika värden)
  int keyMapping[MAX] = {15, 11, 0, 14, 12, 9, 8, 7, 13, 6, 5, 4, 10, 3, 2, 1};

  // Buffert för inmatade siffror
  int inputBuffer[BUFFER] = {0};

  // Index för buffertposition och total summerad siffra
  int bufferIndex = 0, totalSum = 0;

  // Initierar nödvändiga moduler
  t5omsi();              // Startar timer
  colinit();             // Initierar kolumnhantering för tangentbord
  l88init();             // Initierar LED-matris
  keyinit();             // Initierar tangentbordsläsning
  T1powerUpInitPWM(0x1); // Startar PWM för motorstyrning

  // Flagga för att hålla programmet igång
  int programRunning = 1;

  while (programRunning)
  {
    idleCounter++; // Räknar inaktiv tid

    // Kollar om det är dags att utföra en schemalagd händelse
    if (t5expq())
    {
      l88row(colset());       // Uppdaterar tangentbordets rader
      keyPressed = keyscan(); // Skannar tangentbordet för att se om en knapp trycks

      if (keyPressed >= 0)
      {
        // Hanterar olika knapptryckningar med switch-case
        switch (keyMapping[keyPressed])
        {
        // Knappen 'D' - Beräknar ljusstyrka och sätter PWM-värde
        case 15:
          inputBuffer[bufferIndex] = MAX; // Markerar att inmatningen är klar
          for (int i = 0; inputBuffer[i] != MAX; i++)
          {
            // Justerar första siffran om fler än en siffra anges
            if (bufferIndex == 2)
              inputBuffer[0] *= 10;
            else if (bufferIndex == 3)
              inputBuffer[0] *= 100;
            // Summerar inmatade värden för att få rätt procent
            totalSum += inputBuffer[i];
          }
          T1setPWMmotorB(totalSum); // Ställer in PWM för ljusstyrka
          bufferIndex = 0;
          totalSum = 0;
          break;
        // Knappen 'A' - Sätter ljusstyrkan till 100%
        case 10:
          totalSum = 100;
          T1setPWMmotorB(totalSum);
          bufferIndex = 0;
          totalSum = 0;
          break;
        // Knappen '#' - Nollställer all inmatning
        case 11:
          bufferIndex = 0;
          totalSum = 0;
          break;
        // knappen 'c' - Sätter ljusstyrkan till 0%
        case 12:
          totalSum = 0;
          T1setPWMmotorB(totalSum);
          bufferIndex = 0;
          totalSum = 0;
          break;
          // Knappen '*' - Tar bort senaste inmatade siffran
        case 14:
          if (bufferIndex > 0)
            bufferIndex--;
          break;
          // Hanterar siffror (1-9)
        default:
          if (keyMapping[keyPressed] <= 9 && keyPressed >= 1)
          {
            inputBuffer[bufferIndex] = keyMapping[keyPressed]; // Lägger till siffran i bufferten
            bufferIndex++;                                     // Går vidare till nästa position
          }
          break;
        }
      }
    }
  }
}
