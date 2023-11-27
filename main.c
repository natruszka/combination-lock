#include "PIN_LPC17xx.h"
#include "LPC17xx.h"

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "TP_Open1768.h"

#include <stdio.h>
#include <math.h>

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

 void EINT3_init()
 {
	 PIN_Configure(0, 19, 0, 0, 0);
	 LPC_GPIOINT-> IO0IntEnF = (1<<19);
	 NVIC_EnableIRQ(EINT3_IRQn);
	 NVIC_GetActive(EINT3_IRQn);
 }
 
 void EINT3_IRQHandler()
	{
		send("dupa");
		int x;
		int y;
		touchpanelGetXY(&x, &y);
		char temp[10];
		LPC_GPIOINT-> IO0IntEnF = (1<<19);
		NVIC_ClearPendingIRQ(EINT3_IRQn);
		sprintf(temp, "%d, %d \r\n", x, y);
		send(temp);
	}
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
									uint16_t x1, uint16_t y1)
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
			lcdWriteReg(DATA_RAM, LCDBlack);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
 }
 
 /*rysuje krzyzyki, ktorych punkty przeciecia sa 
 w punktach oddalonych o 20 pikseli od rogow*/
 void lcdDrawConfig()
	{
	//lewy gorny rog
	lcdDrawLine(10,20,30,20);
	lcdDrawLine(20,10,20,30);
	
	//prawy gorny rog
	lcdDrawLine(LCD_MAX_X - 20, 10, LCD_MAX_X - 20, 30);
	lcdDrawLine(LCD_MAX_X - 30, 20, LCD_MAX_X - 10, 20);
	
	//lewy dolny rog
	lcdDrawLine(20, LCD_MAX_Y - 10, 20, LCD_MAX_Y - 30);
	lcdDrawLine(10, LCD_MAX_Y - 20, 30, LCD_MAX_Y - 20);
	
	//prawy dolny rog
	lcdDrawLine(LCD_MAX_X - 20, LCD_MAX_Y - 10, LCD_MAX_X - 20, LCD_MAX_Y - 30);
	lcdDrawLine(LCD_MAX_X - 10, LCD_MAX_Y - 20, LCD_MAX_X - 30, LCD_MAX_Y - 20);
 }

/*rysuje interface*/
 void lcdDrawInterface()
{
	int x = LCD_MAX_X / 3;
	int y = LCD_MAX_Y / 3;
	int y_key = y / 3 * 2;
	
	lcdDrawLine(0, y, LCD_MAX_X-1, y);
	lcdDrawLine(0, y+y_key, LCD_MAX_X-1, y+y_key);
	lcdDrawLine(0, y+y_key*2, LCD_MAX_X-1, y+y_key*2);
	lcdDrawLine(x, y, x, LCD_MAX_Y-1);
	lcdDrawLine(x+x, y, x+x, LCD_MAX_Y-1);
	
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
 
 int main(void)
 {
	UART_init();
	EINT3_init();
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
//	char temp[10];
//	sprintf(temp, "%d", LCD_MAX_Y);
//	send(temp);

	while(1) {

	}
 }
