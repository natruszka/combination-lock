#include "PIN_LPC17xx.h"
#include "LPC17xx.h"
#include "GPIO_LPC17xx.h"
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

static double A;
static double B;
static double C;
static double D;
static double E;
static double F;


static void touchpanelDelayUS(uint32_t cnt) {
	volatile uint32_t i;
	for (i = 0; i < cnt; i++) {
		volatile uint8_t us = 12; /*  */
		while (us--) /*  */
		{
			;
		}
	}
}

void send(const char *str)
{
	int i = 0;
	while(str[i] != '\0')
	{
		if(LPC_UART0->LSR & (1 << 5))
		{
			LPC_UART0->THR = str[i++];
		}
	}
}

 void UART_init(void)
 {
 //    LPC_SC->PCONP |= (1 << 25);
     PIN_Configure(0,2,1,2,0);
     PIN_Configure(0,3,1,0,0);

     LPC_UART0->LCR = 3 | (1 << 7);
     LPC_UART0->DLL = 27;
     LPC_UART0->DLM = 0;
     LPC_UART0->LCR = 3;
     LPC_UART0->FCR = 1;
 }

// void EINT3_init()
// {
//	 PIN_Configure(0, 19, 0, 0, 0);
//	 LPC_GPIOINT-> IO0IntEnF = (1<<19);
//	 NVIC_EnableIRQ(EINT3_IRQn);
//	 NVIC_GetActive(EINT3_IRQn);
// }
// 
// void EINT3_IRQHandler()
//	{
//		touchpanelGetXY(&x, &y);
//		char temp[10];
//		LPC_GPIOINT-> IO0IntEnF = (1<<19);
//		NVIC_ClearPendingIRQ(EINT3_IRQn);
//		sprintf(temp, "%d, %d \r\n", x, y);
//		send(temp);
//		cal = true;
//	}
 void lcdWriteChar(uint16_t x_start, uint16_t y_start, unsigned char letter[16])	
	{
		lcdWriteReg(ADRX_RAM, x_start);
		for(uint16_t register y = 0; y < 16; y++)
		{	
		 lcdWriteReg(ADRY_RAM, y+y_start);
		 lcdWriteIndex(DATA_RAM);
		 for(uint16_t register x = 0; x < 8; x++)
		 {
			 (letter[y] & (1 << (8 - x))) ? lcdWriteData(LCDBlack) : lcdWriteData(LCDBlueSea);
		 }
		}
	}
 
 void lcdWriteString(const char* text)
 {
		static uint16_t y_start = 0;
		static uint16_t x_start = 0;
	 unsigned char letter[16] = {'\0'};
	 for(int i = 0; i<strlen(text); i++){
		GetASCIICode(0, letter, text[i]);
		lcdWriteChar(x_start, y_start, letter);
		x_start = x_start + 8;
		if(x_start >= LCD_MAX_X){
			x_start=0;
			y_start += 16;
		}
	}
 }
 
 void lcdDrawLine(uint16_t x0, uint16_t y0, 
									uint16_t x1, uint16_t y1, uint16_t color)
 {
	 if(x1 >= LCD_MAX_X || x0 >=LCD_MAX_X || 
			y1 >= LCD_MAX_Y || y0 >= LCD_MAX_Y )
	 {
				return;
		}
  int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */
 
  for (;;){  /* loop */
			lcdWriteReg(ADRX_RAM, x0);
			lcdWriteReg(ADRY_RAM, y0);
			lcdWriteReg(DATA_RAM, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
 }
 void lcdDrawCross(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
 {
	lcdDrawLine(x - len, y, x + len, y, color);
	lcdDrawLine(x, y - len, x, y + len, color);
 }
 
 /*rysuje krzyzyki, ktorych punkty przeciecia sa 
 w punktach oddalonych o 20 pikseli od rogow*/
 void lcdDrawConfig()
	{
	//lewy gorny rog
		lcdDrawCross(20, 20, 10, LCDBlack);
	
	//prawy gorny rog
		lcdDrawCross(LCD_MAX_X-20, 20, 10, LCDBlack);
		
		//srodek
		lcdDrawCross(LCD_MAX_X/2, LCD_MAX_Y-20, 10, LCDBlack);
 }

/*rysuje interface*/
 void lcdDrawInterface()
{
	int x = LCD_MAX_X / 3;
	int y = LCD_MAX_Y / 3;
	int y_key = y / 3 * 2;
	
	lcdDrawLine(0, y, LCD_MAX_X-1, y, LCDBlack);
	lcdDrawLine(0, y+y_key, LCD_MAX_X-1, y+y_key, LCDBlack);
	lcdDrawLine(0, y+y_key*2, LCD_MAX_X-1, y+y_key*2, LCDBlack);
	lcdDrawLine(x, y, x, LCD_MAX_Y-1, LCDBlack);
	lcdDrawLine(x+x, y, x+x, LCD_MAX_Y-1, LCDBlack);
	
	unsigned char letter[16] = {'\0'};
	
	GetASCIICode(0, letter,'1');
	lcdWriteChar(LCD_MAX_X/2 - x - 4, y*2 - y_key - 8, letter);
	
	GetASCIICode(0, letter,'2');
	lcdWriteChar(LCD_MAX_X/2 - 4, y*2 - y_key - 8, letter);

	GetASCIICode(0, letter,'3');
	lcdWriteChar(LCD_MAX_X/2 + x - 4, y*2 - y_key - 8, letter);
	
	GetASCIICode(0, letter,'4');
	lcdWriteChar(LCD_MAX_X/2 - x - 4, y*2 - 8, letter);
	
	GetASCIICode(0, letter,'5');
	lcdWriteChar(LCD_MAX_X/2 - 4, y*2 - 8, letter);

	GetASCIICode(0, letter,'6');
	lcdWriteChar(LCD_MAX_X/2 + x- 4, y*2 - 8, letter);
	
	GetASCIICode(0, letter,'7');
	lcdWriteChar(LCD_MAX_X/2 - x - 4, y*2 + y_key - 8, letter);
	
	GetASCIICode(0, letter,'8');
	lcdWriteChar(LCD_MAX_X/2 - 4, y*2 + y_key - 8, letter);

	GetASCIICode(0, letter,'9');
	lcdWriteChar(LCD_MAX_X/2 + x- 4, y*2 + y_key - 8, letter);
}

void lcdGetCoords(int *x, int *y)
{
		while(1){
			if(!GPIO_PinRead(0,19)){
				touchpanelGetXY(x,y);
				char temp[10];
				sprintf(temp, "%d, %d \r\n", *x, *y);
				send(temp);
				LPC_GPIOINT-> IO0IntEnF = (1<<19);
				NVIC_ClearPendingIRQ(EINT3_IRQn);
				break;
		}
	}
}

void lcdCallibrate()
{
	int x_arr[3];
	int y_arr[3];
	
	static int callibrated = 0;
	if(callibrated > 2)
	{
		return;
	}
	lcdDrawCross(20, 20, 10, LCDRed);
	lcdGetCoords(&x_arr[0], &y_arr[0]);
	touchpanelDelayUS(2000);
	lcdDrawCross(20, 20, 10, LCDBlack);
	lcdDrawCross(LCD_MAX_X-20, 20, 10, LCDRed);
	lcdGetCoords(&x_arr[1], &y_arr[1]);
	touchpanelDelayUS(2000);
	lcdDrawCross(LCD_MAX_X-20, 20, 10, LCDBlack);
	lcdDrawCross(LCD_MAX_X/2, LCD_MAX_Y-20, 10, LCDRed);
	lcdGetCoords(&x_arr[2], &y_arr[2]);
	touchpanelDelayUS(2000);
	lcdDrawCross(LCD_MAX_X/2, LCD_MAX_Y-20, 10, LCDBlack);
}
 
 int main(void)
 {
	UART_init();
//	EINT3_init();
	touchpanelInit();
	send("START\r\n");
	lcdConfiguration();
	init_ILI9325();
	lcdWriteReg(ADRX_RAM, 0);
	lcdWriteReg(ADRY_RAM, 0);
	lcdWriteIndex(DATA_RAM);
	for(int register x = 0; x < LCD_MAX_X * LCD_MAX_Y; x++)
	{
	 lcdWriteData(LCDBlueSea);
	}
	lcdDrawConfig();
	lcdDrawInterface();
	lcdCallibrate();
	
//	char temp[10];
//	sprintf(temp, "%d", LCD_MAX_Y);
//	send(temp);

	while(1) {

	}
 }
