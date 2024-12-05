/*!
	 @file lcd.h
	 
	 @brief Funkcje do obslugi wyswietlacza alfanumerycznego LCD z uzyciem konwertera TWI - 8 bit.
	 
	 Zawiera funkcje do obslugi wyswietlacza LCD HD44780 poprzez konwerter TWI - 8 bit PCF8574.
	 Funkcje sa skonstruowane pod tryb 4-bitowy, czyli najpierw wysylany jest szereg bitow [7:4], a nastepnie szereg bitow [3:0].
	 
 
	Podlaczenie konwertera z wyswietlaczem:
    * wyswietlacz podlaczony do portu D
    * tryb 4-bitowy
    * linie danych DB7-DB4 dolaczone do PD7-PD4
    * linie sterujace E -> PD2, RS -> PD3

 */

#ifndef LCD_H
#define LCD_H

#include "main.h"
#include <avr/io.h>
#include <util/delay.h>
#include "pcf8574.h"




#define HD44780_CLEaR                   0x01

#define HD44780_HOME                    0x02

#define HD44780_ENTRY_MODE              0x04
   #define HD44780_EM_SHIFT_CURSOR      0
   #define HD44780_EM_SHIFT_DISPLaY     1
   #define HD44780_EM_DECREMENT         0
   #define HD44780_EM_INCREMENT         2

#define HD44780_DISPLaY_ONOFF           0x08
   #define HD44780_DISPLaY_OFF          0
   #define HD44780_DISPLaY_ON           4
   #define HD44780_CURSOR_OFF           0
   #define HD44780_CURSOR_ON            2
   #define HD44780_CURSOR_NOBLINK       0
   #define HD44780_CURSOR_BLINK         1

#define HD44780_DISPLaY_CURSOR_SHIFT    0x10
   #define HD44780_SHIFT_CURSOR         0
   #define HD44780_SHIFT_DISPLaY        8
   #define HD44780_SHIFT_LEFT           0
   #define HD44780_SHIFT_RIGHT          4

#define HD44780_FUNCTION_SET            0x20
   #define HD44780_FONT5x7              0
   #define HD44780_FONT5x10             4
   #define HD44780_ONE_LINE             0
   #define HD44780_TWO_LINE             8
   #define HD44780_4_BIT                0
   #define HD44780_8_BIT                16

#define HD44780_CGRaM_SET               0x40

#define HD44780_DDRaM_SET               0x80


/*!
	@brief wysylanie komendy do wyswietlacza
	
	Funkcja poprzez TWI wysyla komendy do wyswietlacza HD44780. 
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] komendy.
	Nastepnie zawartosc tej zmiennej jest przeslana przez TWI do konwertera.
	Potem przeslany jest tylko stan niski 'Enable', by wczesniej przeslane dane zostaly zapisane w wyswietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozostale 4 bity [3:0] komendy.
	I znowu przeslany jest stan niski 'Enable'
	
	Po tym ma miejsce opoznienie 50 mikrosekund, wedlug zalecen noty katalogowej wyswietlacza.
	
	
	Lista komend konfiguracyjnych (na podstawie noty katalogowej):

	- Czyszczenie wyswietlacza 
		- DB0: 1

	- Powrot na poczatek wyswietlacza 
		- DB1: 1

	- Ustawienie trybu wprowadzania:
		- DB2: 1,
		- DB1: 1 - Inkrementacja,
			- 0 - Dekrementacja;
		- DB0: 1 - Przesuwanie wyswietlacza...
			- DB1: 1 - w lewo,
			- DB1: 0 - w prawo;
		- DB0: 0 - Brak przesuwania wyswietlacza;
	
	- Kontrola wyswietlania
		- DB3: 1,
		- DB2: 1 - Wlaczenie wyswietlacza
			- 0 - Wylaczenie wyswietlacza
		- DB1: 1 - Wyswietlanie kursora
			- 0 - Nie wyswietlanie kursora
		- DB0: 1 - Miganie kursora
			- 0 - Brak migania kursora
	
	- Przesuniecie wyswietlacza/kursora
		- DB4: 1,
		- DB3: 1 - Przesuniecie wyswietlacza...
		- DB2: 1 - ...w prawo
				0 - ...w lewo
		- DB3: 0 - Przesuniecie kursora...
		- DB2: 1 - ...w prawo		(inkrementacja aC)
				0- ...w lewo		(dekrementacja aC)
			
	- Ustawienie sposobu dzialania
		- DB5: 1
		- DB4: 1 - tryb 8 bitowy	(DB7-DB0)
			- 0 - tryb 4 bitowy	(DB7:DB4)
		- DB3: 1 - Uzycie dwoch rzedow do wyswietlania
			- DB2: Wartosc bez znaczenia	
		- DB3: 0 - Jeden wiersz do wyswietlania z ustawieniem czcionki...
			- DB2: 1 - ...5x10 kropek
			- 0 - ...5x8 kropek
			 
	@remark Ta komenda musi byc przeslana na poczatku, poniewaz konfigurowany jest tryb dzialania wyswietlacza: 8-bit i 4-bit.
	Tutaj jest uzywany tryb 4-bitowy, przez co niektore ustawienia nie mialyby skutku.
	W LCD_Init zostalo to wziete pod uwage.
	
	- Ustawianie adresow CGRaM:
		- DB6:1
		-DB5-0:	Sluzy do stworzenia wlasnych znakow w czcionce 8x5
	
	- Ustawianie adresu DDRaM
		- DB7:1
	Ustawienie kursora na konkretne miejsce w wyswietlaczu
	
	@param[in] commandToWrite komenda do wyslania
	
*/
void LCD_WriteCommand(unsigned char commandToWrite);

/*!
	@brief Wyslanie znaku do wyswietlenia
	
	Funkcja poprzez TWI wysyla znak do wyswietlacza HD44780.
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki RS, stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] znaku.
	Nastepnie zawartosc tej zmiennej jest przeslana przez TWI do konwertera.
	Potem przeslany jest tylko stan niski 'Enable', by wczesniej przeslane dane zostaly zapisane w wyswietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozostale 4 bity [3:0] znaku.
	I znowu przeslany jest stan niski 'Enable'
	
	Wyslany znak zostanie wyswietlony zgodnie z ustawieniami trybu wprowadzania.
	
	Po tym ma miejsce opoznienie 50 mikrosekund, wedlug zalecen noty katalogowej wyswietlacza.
	
	@param[in] dataToWrite znak do wyslania
*/
void LCD_WriteData(unsigned char dataToWrite);

/*!
	@brief Wyslanie ciagu znakow do wyswietlenia
	
	Przechodzi po lancuchu znakow wywolujac funkcje LCD_WriteData dla kazdego znaku.
	
	@param[in] *text wskaznik na tablice znakow do wyswietlenia
	@note po przekroczeniu zakresu dlugosci rzedu, pozostaly tekst zostanie wyswietlony w drugim rzedzie.
*/
void LCD_WriteText(char *text);

/*!
	@brief Ustawienie kursora
	
	Ustawienie kursora na wskazana pozycje na wyswietlaczu.
	W jednym rzedzie jest 16 pozycji,
	adresacja pierwszego rzedu zaczyna sie od 0x00, a konczy na 0x0F,
	adresacja drugiego rzedu zaczyna sie od 0x40, a konczy na 0x4F.
	
	@param[in] x numer kolumny/pozycji w rzedzie [0,15].
	@param[in] y numer rzedu [0,1].
*/
void LCD_GoTo(unsigned char x, unsigned char y);

/*!
	@brief Wyczyszczenie ekranu
	
	@note po tej komendzie ma miejsce opoznienie 2 ms.
*/
void LCD_Clear(void);

/*!
	@brief Skierowanie kursora na poczatek oraz przywrocenie ekranu do stanu przed przesunieciem
	
	@remark ustawienie tez stosuje sie do efektow kursora.
*/
void LCD_Home(void);

/*! 
	@brief inicjalizacja ekranu LCD
	
	Wywoluje szereg funkcji LCD_WriteCommand, z predefiniowanymi ustawieniami. Opis ustawien jest w pliku .c
*/
void LCD_Init(void);

#endif//LCD_H
