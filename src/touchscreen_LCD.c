#include "touchscreen_LCD.h"

float A;
float B;
float C;
float D;
float E;
float F;
static bool isConf;

static void touchpanelDelayUS(uint32_t cnt)
{
	volatile uint32_t i;
	for (i = 0; i < cnt; i++)
	{
		volatile uint8_t us = 12; /*  */
		while (us--)
			;
	}
}

void lcdTouchscreenGetCoords(int *x, int *y)
{
	char temp[20];
	while (1)
		if (!GPIO_PinRead(0, 19))
		{
			touchpanelGetXY(x, y);
//			sprintf(temp, "%d, %d \r\n", *x, *y);
//			send(temp);
			if (isConf)
			{
				lcdHandler(*x, *y);
			}
			break;
		}
}

char lcdTouchscreenGetPassword(int *x, int *y, bool showChar, int pos)
{
	char password;
		char temp[20];
	while (1)
		if (!GPIO_PinRead(0, 19))
		{
			sprintf(temp, "%d, %d", *x, *y);
			send(temp);
			touchpanelGetXY(x, y);
			password = lcdGetChar(*x, *y, showChar, pos);
			break;
		}
	return password;
}

int getMin(int cords[], int len)
{
	int min = cords[0];
	for (int i=0; i < len; ++i)
	{
		if (cords[i] < min)
			min = cords[i];
	}
	return min;
}

int getMax(int cords[], int len)
{
	int max = cords[0];
	for (int i =0; i < len; ++i)
	{
		if (cords[i] > max)
			max = cords[i];
	}
	return max;
}

int average(int cords[], int len)
{
	int maxVal = getMax(cords, len);
	int minVal = getMin(cords, len);
	int sum = 0;
	for (int i = 0; i < 10; ++i)
	{
		if (cords[i] == minVal || cords[i] == maxVal)
			continue;
		sum += cords[i];
	}
	return sum / 8;
}

void calculateConstants(int x[], int y[], int len)
{
	int x1 = average(x, len / 3);
	int x2 = average(x + len / 3, len / 3);
	int x3 = average(x + 2 * len / 3, len / 3);

	int y1 = average(y, len / 3);
	int y2 = average(y + len / 3, len / 3);
	int y3 = average(y + 2 * len / 3, len / 3);

	char srednie[40];
	sprintf(srednie, "Srednia 1: %d, %d", x1, y1);
	send(srednie);
	sprintf(srednie, "Srednia 2: %d, %d", x2, y2);
	send(srednie);
	sprintf(srednie, "Srednia 3: %d, %d", x3, y3);
	send(srednie);

	float dzielnik = y1 * (x2-x3)-y2*(x1-x3)+y3*(x1-x2);
	int mozeA = 200 *y1 - 200 *y3;
	int mozeB = -200*x1 +200*x3;
	int mozeC = 20*x2*y1-220*x3*y1 - 20*x1*y2 + 20*x3*y2 + 220*x1*y3 - 20*x2*y3;
	int mozeD = -280*y1 +280*y2;
	int mozeE = 280*x1-280*x2;
	int mozeF = 300*x2*y1 - 20*x3*y1 - 300*x1*y2+20*x3*y2 +20*x1*y3 - 20*x2*y3;
	A = mozeA / dzielnik;
	B = mozeB / dzielnik;
	C = mozeC / dzielnik;
	D = mozeD / dzielnik;
	E = mozeE / dzielnik;
	F = mozeF / dzielnik;

		char temp[70];
		sprintf(temp, "%f, %f, %f, %f, %f, %f", A, B, C, D,E,F);
		send(temp);
		isConf = true;
}

void lcdGetServiceCode(void)
{
	static int pos;
	int x =0;
	int y = 0;
	char code[4];
	char correctCode[4] = {'1','1','1','1'}; // POPRAWNY KOD POWINEN BYC WCZYTANY Z PAMIECI 
	for(int i = 0;i<4;i++)
	{
		send("czekam na haslo");
		code[i] = lcdTouchscreenGetPassword(&x,&y,true, pos);
		touchpanelDelayUS(500000);
		pos++;
	}
	if(checkCode(code, 4, correctCode, 4))
	{
		lcdWriteString("DOBRY KOD", 150, 8);
			lcdClearInput();
	}
	else
	{
		lcdWriteString("ZLY   KOD", 150, 8);
		lcdClearInput();
	}
	pos = 0;
}

// zwiekszanie stosu PAMIETAC DEBILE !!! do 0x00000800
void lcdTouchscreenCallibrate(void)
{
	isConf = false;
	int x_arr[30];
	int y_arr[30];
	unsigned int i;
	int *x_ptr = x_arr;
	int *y_ptr = y_arr;
	for (i = 0; i < 30; ++i)
	{
		x_arr[i] = -1;
		y_arr[i] = -1;
	}
	lcdDrawCross(20, 20, 10, LCDRed);
	for (i = 0; i < 10; ++i)
	{
		send("Jestem w for 1!");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("1 jestem po");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 1");
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, 20, 10, LCDBlack);
	touchpanelDelayUS(2000000);
	lcdDrawCross(LCD_MAX_X - 20, 20, 10, LCDRed);
	for (i = 10; i < 20; ++i)
	{
		send("Jestem w for 2!");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("2 jestem po");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 2");
	touchpanelDelayUS(2000000);
	lcdDrawCross(LCD_MAX_X - 20, 20, 10, LCDBlack);
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, LCD_MAX_Y - 20, 10, LCDRed);
	for (i = 20; i < 30; ++i)
	{
		send("Jestem w for 3!");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("3 jestem po");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 3");
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, LCD_MAX_Y - 20, 10, LCDBlack);
	send("Done");
	touchpanelDelayUS(2000);
	lcdClearScreen();
	calculateConstants(x_arr, y_arr, 30);
}
