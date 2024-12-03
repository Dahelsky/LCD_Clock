/*
	 @brief Funkcje do obslugi wyswietlacza alfanumerycznego LCD z u�yciem konwertera TWI - 8 bit.
	 
	 Zawiera funkcje do obs�ugi wy�wietlacza LCD HD44780 poprzez konwerter TWI - 8 bit PCF8574.
	 Funkcje s� skonstruowane pod tryb 4-bitowy, czyli najpierw wysy�any jest szereg bit�w [7:4], a nast�pnie szereg bit�w [3:0].
	 
 
	Pod��czenie konwertera z wy�wietlaczem:
    * wyswietlacz podlaczony do portu D
    * tryb 4-bitowy
    * linie danych DB7-DB4 dolaczone do PD7-PD4
    * linie sterujace E -> PD2, RS -> PD3

 */

#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>
#include "pcf8574.h"


#define LCD_RS         	0
#define LCD_RW         	1
#define LCD_E         	2
#define LCD_BKLight		3
#define LCD_DATA        4
#define F_CPU 14745000UL

#define HD44780_CLEAR                   0x01

#define HD44780_HOME                    0x02

#define HD44780_ENTRY_MODE              0x04
   #define HD44780_EM_SHIFT_CURSOR      0
   #define HD44780_EM_SHIFT_DISPLAY     1
   #define HD44780_EM_DECREMENT         0
   #define HD44780_EM_INCREMENT         2

#define HD44780_DISPLAY_ONOFF           0x08
   #define HD44780_DISPLAY_OFF          0
   #define HD44780_DISPLAY_ON           4
   #define HD44780_CURSOR_OFF           0
   #define HD44780_CURSOR_ON            2
   #define HD44780_CURSOR_NOBLINK       0
   #define HD44780_CURSOR_BLINK         1

#define HD44780_DISPLAY_CURSOR_SHIFT    0x10
   #define HD44780_SHIFT_CURSOR         0
   #define HD44780_SHIFT_DISPLAY        8
   #define HD44780_SHIFT_LEFT           0
   #define HD44780_SHIFT_RIGHT          4

#define HD44780_FUNCTION_SET            0x20
   #define HD44780_FONT5x7              0
   #define HD44780_FONT5x10             4
   #define HD44780_ONE_LINE             0
   #define HD44780_TWO_LINE             8
   #define HD44780_4_BIT                0
   #define HD44780_8_BIT                16

#define HD44780_CGRAM_SET               0x40

#define HD44780_DDRAM_SET               0x80


/*
	@brief wysy�anie komendy do wy�wietlacza
	
	Funkcja poprzez TWI wysy�a komendy do wy�wietlacza HD44780. 
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] komendy.
	Nastepnie zawarto�� tej zmiennej jest przes�ana przez TWI do konwertera.
	Potem przes�any jest tylko stan niski 'Enable', by wcze�niej przes�ane dane zosta�y zapisane w wy�wietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozosta�e 4 bity [3:0] komendy.
	I znowu przes�any jest stan niski 'Enable'
	
	Po tym ma miejsce op�nienie 50 mikrosekund, wed�ug zalece� noty katalogowej wy�wietlacza.
	
	
	Lista komend konfiguracyjnych (na podstawie noty katalogowej):

	- Czyszczenie wy�wietlacza DB0: 1

	- Powr�t na pocz�tek wy�wietlacza DB1: 1

	- Ustawienie trybu wprowadzania:
	DB2: 1,
	DB1: 1 - Inkrementacja,
		0 - Dekrementacja;
	DB0: 1 - Przesuwanie wy�wietlacza...
		DB1: 1 - w lewo,
			0 - w prawo;
	DB0: 0 - Brak przesuwania wy�wietlacza;
	
	- Kontrola wy�wietlania
	DB3: 1,
	DB2: 1 - W��czenie wy�wietlacza
		0 - Wy��czenie wy�wietlacza
	DB1: 1 - Wy�wietlanie kursora
		0 - Nie wy�wietlanie kursora
	DB0: 1 - Miganie kursora
		0 - Brak migania kursora
	
	- Przesuni�cie wy�wietlacza/kursora
	DB4: 1,
	DB3: 1 - Przesuni�cie wy�wietlacza...
		DB2: 1 - ...w prawo
			0 - ...w lewo
	DB3: 0 - Przesuni�cie kursora...
		DB2: 1 - ...w prawo		(inkrementacja AC)
			0 - ...w lewo		(dekrementacja AC)
			
	- Ustawienie sposobu dzia�ania
	DB5: 1
	DB4: 1 - tryb 8 bitowy	(DB7-DB0)
		0 - tryb 4 bitowy	(DB7:DB4)
	DB3: 1 - U�ycie dw�ch rz�d�w do wy�wietlania
		DB2: Warto�� bez znaczenia	
	DB3: 0 - Jeden wiersz do wy�wietlania z ustawieniem czcionki...
		DB2: 1 - ...5x10 kropek
			 0 - ...5x8 kropek
			 
	@Remark Ta komenda musi by� przes�ana na pocz�tku, poniewa� konfigurowany jest tryb dzia�ania wy�wietlacza: 8-bit i 4-bit.
	Tutaj jest u�ywany tryb 4-bitowy, przez co niekt�re ustawienia nie mia�yby skutku.
	W LCD_Init zosta�o to wzi�te pod uwag�.
	
	- Ustawianie adres�w CGRAM:
	DB6:1
	DB5-0:	S�u�y do stworzenia w�asnych znak�w w czcionce 8x5
	
	- Ustawianie adresu DDRAM
	DB7:1
	Ustawienie kursora na konkretne miejsce w wy�wietlaczu
	
	@param[in] commandToWrite komenda do wys�ania
	
*/
void LCD_WriteCommand(unsigned char commandToWrite);

/*
	@brief Wys�anie znaku do wy�wietlenia
	
	Funkcja poprzez TWI wysy�a znak do wy�wietlacza HD44780.
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki RS, stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] znaku.
	Nastepnie zawarto�� tej zmiennej jest przes�ana przez TWI do konwertera.
	Potem przes�any jest tylko stan niski 'Enable', by wcze�niej przes�ane dane zosta�y zapisane w wy�wietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozosta�e 4 bity [3:0] znaku.
	I znowu przes�any jest stan niski 'Enable'
	
	Wys�any znak zostanie wy�wietlony zgodnie z ustawieniami trybu wprowadzania.
	
	Po tym ma miejsce op�nienie 50 mikrosekund, wed�ug zalece� noty katalogowej wy�wietlacza.
	
	@param[in] dataToWrite znak do wys�ania
*/
void LCD_WriteData(unsigned char dataToWrite);

/*
	@brief Wys�anie ci�gu znak�w do wy�wietlenia
	
	Przechodzi po �a�cuchu znak�w wywo�uj�c funkcj� LCD_WriteData dla ka�dego znaku.
	
	@param[in] *text wska�nik na tablic� znak�w do wy�wietlenia
	@note po przekroczeniu zakresu d�ugo�ci rz�du, pozosta�y tekst zostanie wy�wietlony w drugim rz�dzie.
*/
void LCD_WriteText(char *text);

/*
	@brief Ustawienie kursora
	
	Ustawienie kursora na wskazan� pozycj� na wy�wietlaczu.
	W jednym rz�dzie jest 16 pozycji,
	adresacja pierwszego rz�du zaczyna si� od 0x00, a ko�czy na 0x0F,
	adresacja drugiego rz�du zaczyna sie od 0x40, a ko�czy na 0x4F.
	
	@param[in] x numer kolumny/pozycji w rz�dzie [0,15].
	@param[in] y numer rz�du [0,1].
*/
void LCD_GoTo(unsigned char x, unsigned char y);

/*
	@brief Wyczyszczenie ekranu
	
	@note po tej komendzie ma miejsce op�nienie 2 ms.
*/
void LCD_Clear(void);

/*
	@brief Skierowanie kursora na pocz�tek oraz przywr�cenie ekranu do stanu przed przesuni�ciem
	
	@remark ustawienie te� stosuje si� do efekt�w kursora.
*/
void LCD_Home(void);

/* 
	@brief inicjalizacja ekranu LCD
	
	Wywo�uje szereg funkcji LCD_WriteCommand, z predefiniowanymi ustawieniami. Opis ustawie� jest w pliku .c
*/
void LCD_Init(void);

#endif//LCD_H
