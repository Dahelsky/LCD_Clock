#include "LCD.h"
#include <util/delay.h>
#include <avr/io.h>
void LCD_Command(unsigned char cmd)
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

void LCD_Data(unsigned char data)
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

void LCD_Text(const char* text)
{
	while (*text)
		LCD_Data(*text++);
}

void LCD_Init()
{
	LCD_DDR = 0xFF; //output
	LCD_PORT = 0x00; //RS = 0, E = 0;
	
	LCD_Command(1<<1); //Return home
	LCD_Command((1<<5)|(0<<3)|(1<<0));//Function set
	LCD_Command(1<<0); //Clear Display
	LCD_Command((1<<2)|(1<<1)); //Entry mode 
	LCD_Command((1<<3)|(1<<2)); //Display On/Off Control
	
	//LCD_Command(0x02);
	//LCD_Command(0x28);
	//LCD_Command(0x01);
	//LCD_Command(0x0F);
	//LCD_Command(0x06);
}

void LCD_Clear()
{
	LCD_Command(1<<1);
}