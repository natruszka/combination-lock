#include "touchscreen_LCD.h"

int A;
int B;
int C;
int D;
static bool isConf;

static void touchpanelDelayUS(uint32_t cnt)
{
	volatile uint32_t i;
	for ( i = 0; i < cnt; i++ )
	{
		volatile uint8_t us = 12; /*  */
		while ( us-- );
	}
}

void lcdTouchscreenGetCoords(int *x, int *y)
{
	char temp[20];
	while (1)
		if (!GPIO_PinRead(0, 19))
		{
			touchpanelGetXY(x, y);
			sprintf(temp, "%d, %d \r\n", *x, *y);
			send(temp);
			if (isConf)
			{
				lcdHandler(*x, *y);
			}
			break;
		}
}

int getMin(int cords[], int len)
{
	int min = cords[0];
	for (int i; i < len; ++i)
	{
		if (cords[i] < min)
			min = cords[i];
	}
	return min;
}

int getMax(int cords[], int len)
{
	int max = cords[0];
	for (int i; i < len; ++i)
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
	int x1 = average(x, len/3);
	int x2 = average(x + len / 3, len/3);
	int x3 = average(x + 2 * len / 3, len/3);

	int y1 = average(y, len/3);
	int y2 = average(y + len / 3, len/3);
	int y3 = average(y + 2 * len / 3, len/3);

	int x1subx3 = x1 - x3;
	int y3suby1 = y3 - y1;
	int Bdivider = x2 * y3suby1 / x1subx3 + y2;
	B = 220 / Bdivider;
	A = B * y3suby1 / x1subx3;

	int y2suby1 = y2 - y1;
	int x1subx2 = x1 - x2;
	int Ddivider = x3 * y2suby1 / x1subx2 + y3;
	D = 300 / Ddivider;
	C = D * y2suby1 / x1subx2;
	isConf = true;
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
		send("Jestem w for 1!\n\r");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("1 jestem po\n\r");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 1\n\r");
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, 20, 10, LCDBlack);
	touchpanelDelayUS(2000000);
	lcdDrawCross(LCD_MAX_X - 20, 20, 10, LCDRed);
	for (i = 10; i < 20; ++i)
	{
		send("Jestem w for 2!\n\r");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("2 jestem po\n\r");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 2\n\r");
	touchpanelDelayUS(2000000);
	lcdDrawCross(LCD_MAX_X - 20, 20, 10, LCDBlack);
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, LCD_MAX_Y - 20, 10, LCDRed);
	for (i = 20; i < 30; ++i)
	{
		send("Jestem w for 3!\n\r");
		lcdTouchscreenGetCoords(x_ptr++, y_ptr++);
		send("3 jestem po\n\r");
		touchpanelDelayUS(2000);
	}
	send("czekam po for 3\n\r");
	touchpanelDelayUS(2000000);
	lcdDrawCross(20, LCD_MAX_Y - 20, 10, LCDBlack);
	send("Done\r\n");
	touchpanelDelayUS(2000);
	lcdClearScreen();
	calculateConstants(x_arr, y_arr, 30);
}
