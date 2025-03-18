#include "gd32vf103.h" // Bibliotek för GD32VF103-mikrokontroller
#include "drivers.h"   // Drivrutiner för hårdvara
#include "adc.h"       // ADC-funktioner (t.ex. för potentiometer)
#include "pwm.h"       // PWM-funktioner
#include <stdlib.h>    // Standardbibliotek för slumpmässiga tal
#include <time.h>      // Bibliotek för tidshantering
#define GRID 8         // LED-matrisens storlek (8x8)
#define ROWS 48        // Antal fiendebilar (exempel med 3 bilar)
#define HEX 0xff       // 1111 1111 för resultat.
#define ADC
/*########################################################################*/
// Globala variabler
/*########################################################################*/
int milliseconds = 0;
int adcValue;                   // ADC-avläsning
int score = 0;                  // Spelarens poäng
int gameOver = 0;               // Flagga för att indikera game over
int playerColumn = 4;           // start position.
int scrollIndex = 0;            // flagga för att hålla reda vart fordonen skrollas.
unsigned char gameBoard[ROWS] = // 8bitar- binärt
    {
        // Första blocket (rader 0-23) // 8 rader tomma
        // Tre bilar
        0b00000000, // rad 0
        0b00000000, // rad 1
        0b00000000, // rad 2
        0b00000000, // rad 3
        0b00000000, // rad 4
        0b00110100, // rad 5
        0b00000000, // rad 6
        0b00000000, // rad 7

        // En buss + 2mc
        0b00001100, // rad 8
        0b00001100, // rad 9
        0b00001100, // rad 10
        0b00001100, // rad 11
        0b00000010, // rad 12
        0b00100010, // rad 13
        0b00100000, // rad 14
        0b00000000, // rad 15

        // väg arbete
        0b00000000, // rad 16
        0b01111110, // rad 17
        0b01000010, // rad 18
        0b01001010, // rad 19
        0b01000010, // rad 20
        0b01111110, // rad 21
        0b00000000, // rad 22
        0b00000000, // rad 23

        // Andra blocket (rader 24-47), repeterar mönstret
        // MC
        0b00000001, // rad 24
        0b00000001, // rad 25
        0b00001000, // rad 26
        0b00101000, // rad 27
        0b00100000, // rad 28
        0b00000000, // rad 29
        0b00100000, // rad 30
        0b00100000, // rad 31

        // Bil (8 rader)
        0b00000000, // rad 32
        0b00000000, // rad 33
        0b00000000, // rad 34
        0b00111100, // rad 35
        0b00111100, // rad 36
        0b00111100, // rad 37
        0b00000000, // rad 38
        0b00000000, // rad 39

        // Buss (8 rader)
        0b00110011, // rad 40
        0b00000000, // rad 41
        0b00000000, // rad 42
        0b00000000, // rad 43
        0b00000000, // rad 44
        0b00000000, // rad 45
        0b00000000, // rad 46
        0b00000000  // rad 47
};

/*########################################################################*/
// Enum för spelets tillstånd
/*########################################################################*/
typedef enum
{
  STATE_PLAY,    // Spelet är igång
  STATE_GAMEOVER // Game over
} GameState;

/*########################################################################*/
// Funktioner.
/*########################################################################*/

// Funktion för att hantera spelarens bil baserat på potentiometeravläsning.
void playerCarAndMove(void)
{
  // Omvandla ADC-värdet (0-4095) till en position mellan 0 och 7
  playerColumn = (adcValue * GRID) / ADC;

  // Säkerställ att positionen ligger inom intervallet 0-7
  if (playerColumn < 0)
    playerColumn = 0;
  if (playerColumn > 7)
    playerColumn = 7;

  // Rita ut spelarens bil på rad 0 med en bitmask som sätter på rätt kolumn
  l88mem(0, 1 << playerColumn);
}
// Funktion för att uppdatera fiendebilarnas positioner och rita ut dem.
void gameScroll(void)
{
  for (int i = 0; i < GRID; i++)
  {
    int boardRow = (scrollIndex + i) % ROWS; // beräkna vilken rad
    l88mem(i, gameBoard[boardRow]);
  }
  scrollIndex = (scrollIndex + 1) % ROWS; // flytta den neråt
}
// checka på krasch
int checkCollision(void)
{
  int currentVehicleRow = scrollIndex; // Eftersom rad 0 visas med gameBoard[scrollIndex]
  // Om fiendebilens mönster har en bit satt på spelarens position sker en kollision
  if (gameBoard[currentVehicleRow] & (1 << playerColumn))
  {
    return 1;
  }
  return 0;
}

// printa resultat funktion
void printScore(void)
{
  // kanske måste rensa raderna
  l88mem(4, (unsigned char)score);
}

/*########################################################################*/
// hela spelet.
/*########################################################################*/
int main(void)
{
  // Initiera hårdvara
  t5omsi();           // Initierar Timer 5 (1 kHz)
  colinit();          // Initierar kolumnhantering för LED-matrisen
  l88init();          // Initierar 8x8 LED-matrisen
  ADC3powerUpInit(1); // Initierar ADC för potentiometer

  GameState gameState = STATE_PLAY; // Starta spelet i spel-läget

  while (1)
  {
    switch (gameState)
    {
    case STATE_PLAY:
      // Läs ADC om flaggan är satt
      if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET)
      {
        adcValue = adc_regular_data_read(ADC0);
        adc_flag_clear(ADC0, ADC_FLAG_EOC);
      }
      // Om timerintervall har löpt ut (1 ms per tick)
      if (t5expq())
      {
        milliseconds += 1;

        // Var 500:e millisekund uppdateras fiendebilarnas positioner
        if (milliseconds >= 200)
        {
          milliseconds = 0;
          gameScroll();
          score++;
        }

        playerCarAndMove(); // Hantera spelarens bil utifrån ADC-avläsningen

        if (checkCollision())
        {
          gameOver = 1;
        }

        l88row(colset()); // Uppdatera LED-matrisens radstyrning (multiplexering)

        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // Trigga en ny ADC-avläsning
      }

      // Om en kollision har inträffat, ändra spelets tillstånd
      if (gameOver)
      {
        gameState = STATE_GAMEOVER;
      }
      break;

    case STATE_GAMEOVER:
      printScore(); // Hantera game over, t.ex. visa poäng eller en animation
      return 0;     // Avslutar programmet i detta exempel
    default:
      // Säkerställ att vi hamnar i ett giltigt tillstånd
      break;
    }
  }
}