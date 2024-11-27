#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>

#define RS					0	//pin nr, values: RS = 1 - Data, RS = 0 - Command
#define E					1
#define LCD_PORT			PORTB
#define LCD_DDR				DDRB

#define F_CPU 16000000UL


void LCD_command(unsigned char cmd);
void LCD_data(unsigned char data);
void LCD_text(const char* text);
void LCD_init();

#endif /* LCD_H_ */