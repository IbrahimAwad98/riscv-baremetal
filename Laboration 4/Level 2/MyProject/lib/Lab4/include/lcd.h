// Drivrutin för LCD-skärmen på Longan Nano
/*
  Longan Nano LCD driver

*/

#ifndef __LCD_H // Start av include guard: förhindrar att filen inkluderas flera gånger
#define __LCD_H

#include "stdlib.h"         // Inkluderar standardbiblioteket för generella funktioner, t.ex. minneshantering
#include "gd32vf103_gpio.h" // Inkluderar GPIO-specifika definitioner för GD32VF103-mikrokontrollern
// LCD-lägen
#define LCD_NORMAL 0   // Normal visning
#define LCD_INVERTED 1 // Inverterad visning (färger/pixlar vänds)
// Skärmupplösning
#define LCD_W 160 // Skärmens bredd i pixlar
#define LCD_H 80  // Skärmens höjd i pixlar
// Makron för LED-styrning (definierade men ej med värde, kan utökas vid behov)
#define LED_ON
#define LED_OFF
//----------------- OLED Port definition ----------------
// Chip Select (CS) styrs via GPIOC pin 13
#define OLED_CS_Clr() gpio_bit_reset(GPIOC, GPIO_PIN_13) // Sätter CS-linjen låg
#define OLED_CS_Set() gpio_bit_set(GPIOC, GPIO_PIN_13)   // Sätter CS-linjen hög
// Observera: Kommentaren anger "CS PB2", vilket kan innebära att hårdvarumappningen bör kontrolleras
// Data/Command (DC) styrs via GPIOC pin 15
#define OLED_DC_Clr() gpio_bit_reset(GPIOC, GPIO_PIN_15) // Sätter DC-linjen låg (indikera kommando)
#define OLED_DC_Set() gpio_bit_set(GPIOC, GPIO_PIN_15)   // Sätter DC-linjen hög (indikera data)
// Observera: Kommentaren anger "DC PB0", så kontrollera att port och pin stämmer
// Makron för att ange om vi skickar kommando eller data till LCD:n
#define OLED_CMD 0  // Kommando
#define OLED_DATA 1 // Data
// Makron för att hantera bakgrund vid text-/grafikritytor
#define TRANSPARENT 1 // Transparent bakgrund
#define OPAQUE 0      // Ogenomskinlig (solid) bakgrund

// Typalias för enklare hantering av dataformat
typedef unsigned char u8;  // 8-bitars osignerad typ
typedef unsigned int u16;  // 16-bitars osignerad typ
typedef unsigned long u32; // 32-bitars osignerad typ
// Deklaration av en extern variabel för bakgrundsfärg (definieras i .c-filen)
extern u16 BACK_COLOR;

// Funktioner för LCD-hantering
// Funktioner för att hantera kö för skrivoperationer
void LCD_WR_Queue();
void LCD_Wait_On_Queue();

// Funktioner för att skriva data via bussgränssnittet
void LCD_Write_Bus(int dat); // Skicka data via buss med int-typ
void LCD_Writ_Bus(u8 dat);   // Skicka 8-bitars data via buss

// Funktioner för att skriva data med olika bredd
void LCD_WR_DATA8(u8 dat); // Skicka 8-bitars data
void LCD_WR_DATA(u16 dat); // Skicka 16-bitars data

void LCD_WR_REG(u8 dat); // Skicka ett kommando till LCD:n

// Sätt det aktiva ritområdet på skärmen (koordinater: x1,y1 till x2,y2)
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2);

// Ställ in LCD-typ (t.ex. normal eller inverterad)
void Lcd_SetType(int type);

// Initiera LCD-skärmen
void Lcd_Init(void);

// Rensa skärmen med angiven färg
void LCD_Clear(u16 Color);

// Visa kinesiska tecken på skärmen
void LCD_ShowChinese(u16 x, u16 y, u8 index, u8 size, u16 color);

// Rita punkter
void LCD_DrawPoint(u16 x, u16 y, u16 color);     // Rita en enstaka punkt
void LCD_DrawPoint_big(u16 x, u16 y, u16 color); // Rita en större punkt

// Fyll ett rektangulärt område med en färg
void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, u16 color);

// Rita en linje mellan två punkter
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

// Rita en rektangel
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);

// Rita en cirkel med centrum (x0,y0) och radie r
void Draw_Circle(u16 x0, u16 y0, u8 r, u16 color);

// Visa tecken och strängar på LCD:n
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 mode, u16 color);     // Visa ett tecken
void LCD_ShowString(u16 x, u16 y, const u8 *p, u16 color);       // Visa en sträng
void LCD_ShowStr(u16 x, u16 y, const u8 *p, u16 color, u8 mode); // Visa en sträng med specificerat läge

// Visa nummer på skärmen
void LCD_ShowNum(u16 x, u16 y, u16 num, u8 len, u16 color);    // Visa ett heltal med fast längd
void LCD_ShowNum1(u16 x, u16 y, float num, u8 len, u16 color); // Visa ett flyttal med fast längd

// Visa en bild på skärmen, definierad som en array med pixlar
void LCD_ShowPicture(u16 x1, u16 y1, u16 x2, u16 y2, u8 *image);

// Visa en logotyp på skärmen
void LCD_ShowLogo(u16 y);

// printa resultat
void printPercentage(const u8 *percentageText);

// Beräkna potens (m^n)
u32 mypow(u8 m, u8 n);

//---------------------- Färgdefinitioner -------------------------
// Färger i RGB565-format (16-bitars), vanligt för LCD-skärmar
#define WHITE 0xFFFF      // Vit
#define BLACK 0x0000      // Svart
#define BLUE 0x001F       // Blå
#define BRED 0XF81F       // Röd (kan indikera "blodröd")
#define GRED 0XFFE0       // Alternativ röd/grön nyans
#define GBLUE 0X07FF      // Alternativ blå/grön nyans
#define RED 0xF800        // Röd
#define MAGENTA 0xF81F    // Magenta (samma värde som BRED)
#define GREEN 0x07E0      // Grön
#define CYAN 0x7FFF       // Cyan
#define YELLOW 0xFFE0     // Gul (samma värde som GRED, dubbelkontrollera vid användning)
#define BROWN 0XBC40      // Brun
#define BRRED 0XFC07      // Mörkröd/brun nyans
#define GRAY 0X8430       // Grå
#define DARKBLUE 0X01CF   // Mörkblå
#define LIGHTBLUE 0X7D7C  // Ljusblå
#define GRAYBLUE 0X5458   // Gråblå
#define LIGHTGREEN 0X841F // Ljusgrön
#define LGRAY 0XC618      // Ljusgrå
#define DGRAY 0X31A6      // Mörkgrå
#define LGRAYBLUE 0XA651  // Ljusgråblå
#define LBBLUE 0X2B12     // Alternativ ljusblå

#endif // __LCD_H