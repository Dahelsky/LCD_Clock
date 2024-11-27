#include <avr/io.h>
#include "LCD.h"


int main(void)
{
    LCD_init();
    LCD_text("abc");
    while (1) 
    {
    }
	
}

