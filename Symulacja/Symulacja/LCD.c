#include "LCD.h"
#include <util/delay.h>
#include <avr/io.h>
void LCD_command(unsigned char cmd)
{
	LCD_PORT &= ~(1<<RS);
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0);
	LCD_PORT |= (1<<E);
	_delay_us(2);
	LCD_PORT &= ~(1<<E);
	
	_delay_us(20);
	LCD_PORT = (LCD_PORT & 0x0F) | (cmd<<4);
	LCD_PORT |= (1<<E);
	_delay_us(2);
	LCD_PORT &= ~(1<<E);
	_delay_us(50);
	
	
}

void LCD_data(unsigned char data)
{
	LCD_PORT |= (1<<RS);
	LCD_PORT |= (1<<E);
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
	_delay_us(2);
	LCD_PORT &= ~(1<<E);
	
	_delay_us(20);
	LCD_PORT = (LCD_PORT & 0x0F) | (data<<4);
	LCD_PORT |= (1<<E);
	_delay_us(2);
	LCD_PORT &= ~(1<<E);
	_delay_us(50);
}

void LCD_text(const char* text)
{
	while (*text)
		LCD_data(*text++);
}

void LCD_init()
{
	LCD_DDR = 0xFF; //output
	LCD_PORT = 0x00; //RS = 0, E = 0;
	LCD_command(0x02);
	LCD_command(0x28);
	LCD_command(0x01);
	LCD_command(0x0F);
	LCD_command(0x06);
}