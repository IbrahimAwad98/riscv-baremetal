#include "gd32vf103.h"
#include "drivers.h"
#include "adc.h"
#include "lcd.h"
#include "usart.h"
#define INTERRUPTS_ENABLED 1
#define INTERRUPTS_DISABLED 0
#define MAX 16
#define BUFFER 20

int main(void)
{
  /*################ Variabeldeklaration #############################*/
  int keyPressed;                                                              // Variabel för att lagra knapptryck
  int lookUpTbl[MAX] = {13, 15, 0, 14, 12, 9, 8, 7, 11, 6, 5, 4, 10, 3, 2, 1}; // Lookup-tabell för knappmappning
  int adcRegularValue = 0;                                                     // Variabel för ADC-data (vanlig kanal)
  int adcInsertedValue = 0;                                                    // Variabel för ADC-data (insatt kanal)
  char messageButtonB[] = "ID-000\n";                                          // Vid knapptryck B
  char messageButtonC[] = "ID*000\n";                                          // Vid knapptryck C
  char messageButtonA[] = "ID+0  \n";                                          // Vid knapptryck A
  char lcdDisplayBuffer[BUFFER] = {0};                                         // Buffert för mottagen USART-data som ska visas på LCD
  int lcdBufferIndex = 0;                                                      // vart vi är i buffert
  int buttonA_PressedFlag = 0;                                                 // Flagga som indikerar att knapp A har tryckts (för temperaturmätning)
  int temperature = 0;                                                         // Variabel för temperaturvärdet

  /*################ Initiera hårdvaran #############################*/
  t5omsi();                                                // Initiera timer5 med 1kHz tick
  colinit();                                               // Initiera kolumnhantering (för LED-matris)
  keyinit();                                               // Initiera tangentbords-/knappgränssnittet
  ADC3powerUpInit(1);                                      // Initiera ADC-kanal (ADC0, kanal 3)
  Lcd_SetType(LCD_NORMAL);                                 // Välj normal displayläge (alternativt LCD_INVERTED)
  Lcd_Init();                                              // Initiera lcd
  LCD_Clear(BLACK);                                        // sätt den svart
  u0init(INTERRUPTS_ENABLED);                              // Initiera USART0 (seriell kommunikation)
  eclic_global_interrupt_enable();                         // Aktivera globala avbrott
  LCD_ShowStr(10, 10, "LAB5: LEVEL2", WHITE, TRANSPARENT); // Visa en header-text på LCD (valfritt)

  /*################ NIVÅ 2 #############################*/
  while (1)
  {
    LCD_WR_Queue(); // Hantera LCD-kommandokö
    // ADC-hantering
    if (adc_flag_get(ADC0, ADC_FLAG_EOC) == SET)
    {
      // sätt på kanal 3
      if (adc_flag_get(ADC0, ADC_FLAG_EOIC) == SET)
      {
        adcInsertedValue = adc_inserted_data_read(ADC0, ADC_INSERTED_CHANNEL_0);

        // Om knapp A har tryckts: beräkna temperatur
        if (buttonA_PressedFlag)
        {
          // Temperaturberäkning (omvandla ADC till temp)
          temperature = ((0x680 - adcInsertedValue) / 5) + 25;
          buttonA_PressedFlag = 0; // Återställ flaggan

          // Uppdatera meddelandet med beräknad temperatur
          messageButtonA[4] = '0' + temperature / 10;
          messageButtonA[5] = '0' + temperature % 10;
          putstr(messageButtonA);
        }
        // Rensa ADC-flaggorna
        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        adc_flag_clear(ADC0, ADC_FLAG_EOIC);
      }
      else
      {
        // Hantera vanlig ADC-konverteringsdata
        adcRegularValue = adc_regular_data_read(ADC0);
        adc_flag_clear(ADC0, ADC_FLAG_EOC);
        adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // Trigga ny ADC-konvertering för den vanliga kanalen
      }
    }
    // USART-hantering (TX till RX)
    if (usart_flag_get(USART0, USART_FLAG_RBNE))
    {
      char receivedChar = usart_data_receive(USART0);
      // Om ett newline-meddelande mottagits: bearbeta hela strängen
      if (receivedChar == '\n')
      {
        LCD_Clear(BLACK);
        LCD_ShowStr(10, 10, lcdDisplayBuffer, WHITE, BLACK);

        //  Kontrollera om meddelandet följer formatet "ID" följt av '+' eller '-'
        if (lcdDisplayBuffer[0] == 'I' && lcdDisplayBuffer[1] == 'D' &&
            (lcdDisplayBuffer[2] == '+' || lcdDisplayBuffer[2] == '-'))
        {
          LCD_ShowStr(70, 10, "OK!", GREEN, TRANSPARENT);
        }
        else
        {
          LCD_ShowStr(70, 10, "ERR!", RED, TRANSPARENT);
        }
        // Nollställ bufferten
        lcdBufferIndex = 0;
        for (int i = 0; i < BUFFER; i++)
        {
          lcdDisplayBuffer[i] = '\0';
        }
      }
      else
      {
        // Lägg endast till tecken om det finns plats i bufferten
        if (lcdBufferIndex < (int)(sizeof(lcdDisplayBuffer) - 1))
        {
          lcdDisplayBuffer[lcdBufferIndex++] = receivedChar;
        }
      }
    }
    // keyboard
    if (t5expq())
    {
      l88row(colset());
      // Skanna efter knapptryck
      if ((keyPressed = keyscan()) >= 0)
      {
        // Mappa knappindex med hjälp av lookUpTbl
        int mappedKey = lookUpTbl[keyPressed];
        switch (mappedKey)
        {
        case 10:
          // Knapp A tryckt: sätt flagga för att senare mäta temperatur
          buttonA_PressedFlag = 1;
          break;
        case 11:
          // Knapp B tryckt: skicka meddelande för knapp B
          putstr(messageButtonB);
          break;
        case 12:
          // Knapp C tryckt: skicka meddelande för knapp C
          putstr(messageButtonC);
          break;
        }
      }
      // Trigga nästa ADC-konvertering för den vanliga kanalen
      adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    }
  }
  return 0;
}