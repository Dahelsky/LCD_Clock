#ifndef LCD_H_
#define LCD_H_
#include <avr/io.h>

#define RS					0	//pin nr, values: RS = 1 - Data, RS = 0 - Command
#define E					1
#define LCD_PORT			PORTB
#define LCD_DDR				DDRB


#define F_CPU 16000000UL

/*
List of commands:

	- Clear Display DB0: 1

	- Return home DB1: 1

	- Entry mode set:
		DB2: 1;
		DB1: 1 - Increment,
			 0 - Decrement;
		DB0: 1 - Shift display...
			DB1: 1 - to the left,
				 0 - to the right;
		DB0: 0 - Don't shift display;
		 
	- Display On/Off Control
		DB3: 1,
		DB2: 1 - Turn on display
			 0 - Turn off display
		DB1: 1 - Display cursor
			 0 - Don't display cursor
		DB0: 1 - Cursor blinks
			 0 - Cursor doesn't blink
		 
	- Cursor or display shift
		DB4: 1,
		DB3: 1 - Display shift...
			DB2: 1 - ...to the right
				 0 - ...to the left
		DB3: 0 - Cursor shift...
			DB2: 1 - ...to the right	(AC increment)
				 0 - ...to the left		(AC decrement)
	- Function set
		DB5: 1	
		DB4: 1 - 8 bit mode (DB7-DB0)
			 0 - 4 bit mode	(DB7:DB4)
		DB3: 1 - Two display lines
			DB2: Don't care
		DB3: 0 - One display line with font setting...
			DB2: 1 - 5x10 dots
				 0 - 5x8 dots
		Note: Send this instruction first
	- There are also instructions for:
		Setting CGRAM address
		Setting DDRAM address
		Reading busy flag and addresses
		but I probably don't need them for now so I won't bother with writing them here. 
		Check the docs if you want what are they for
*/
void LCD_Command(unsigned char cmd);

void LCD_Data(unsigned char data);
void LCD_Text(const char* text);
void LCD_Init();
void LCD_Clear();


#endif /* LCD_H_ */