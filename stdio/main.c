#include "main.h"
#include <util/delay.h>
#include <avr/io.h>
#include "pcf8574.h"
#include "twi.h"
#include "lcd.h"
#include "systimer.h"

#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>

FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);
static int czas[] = {2,2,1,5,2,3};
void systimer_callback()
{
	if(++czas[2]>=60)
	{
		if(++czas[1]>=60)
		{
			if(++czas[0]>=24)
				czas[0]=0;
				
			czas[1]=0;
		}
		czas[2]=0;
	}
	LCD_Clear();
	
	fprintf_P(&LCDstdout, PSTR("%i:%i:%i"), czas[0],czas[1],czas[2]);
}

int main(void)
{
	systimer_init(1000);//1sek
	
	TWI_init();
	LCD_Init();
	
	//stdout = fdevopen((void *)LCD_WriteData, NULL);
	//stdout = &LCDstdout;
	
	//uint8_t var1 = 125;
	//float var2 = 12.63;
	
	fprintf_P(&LCDstdout, PSTR("%i:%i:%i"), czas[0],czas[1],czas[2]);

	
	
	//printf(czas[2]);
	//LCD_GoTo(0,1); //druga linia
	//printf("have some oats");
	
	
	//printf(var2);
	while (1) 
  {
	  
  }
  
}
