/*
	 @brief Funkcje do obslugi wyswietlacza alfanumerycznego LCD z u¿yciem konwertera TWI - 8 bit.
	 
	 Zawiera funkcje do obs³ugi wyœwietlacza LCD HD44780 poprzez konwerter TWI - 8 bit PCF8574.
	 Funkcje s¹ skonstruowane pod tryb 4-bitowy, czyli najpierw wysy³any jest szereg bitów [7:4], a nastêpnie szereg bitów [3:0].
	 
 
	Pod³¹czenie konwertera z wyœwietlaczem:
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
	@brief wysy³anie komendy do wyœwietlacza
	
	Funkcja poprzez TWI wysy³a komendy do wyœwietlacza HD44780. 
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] komendy.
	Nastepnie zawartoœæ tej zmiennej jest przes³ana przez TWI do konwertera.
	Potem przes³any jest tylko stan niski 'Enable', by wczeœniej przes³ane dane zosta³y zapisane w wyœwietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozosta³e 4 bity [3:0] komendy.
	I znowu przes³any jest stan niski 'Enable'
	
	Po tym ma miejsce opóŸnienie 50 mikrosekund, wed³ug zaleceñ noty katalogowej wyœwietlacza.
	
	
	Lista komend konfiguracyjnych (na podstawie noty katalogowej):

	- Czyszczenie wyœwietlacza DB0: 1

	- Powrót na pocz¹tek wyœwietlacza DB1: 1

	- Ustawienie trybu wprowadzania:
	DB2: 1,
	DB1: 1 - Inkrementacja,
		0 - Dekrementacja;
	DB0: 1 - Przesuwanie wyœwietlacza...
		DB1: 1 - w lewo,
			0 - w prawo;
	DB0: 0 - Brak przesuwania wyœwietlacza;
	
	- Kontrola wyœwietlania
	DB3: 1,
	DB2: 1 - W³¹czenie wyœwietlacza
		0 - Wy³¹czenie wyœwietlacza
	DB1: 1 - Wyœwietlanie kursora
		0 - Nie wyœwietlanie kursora
	DB0: 1 - Miganie kursora
		0 - Brak migania kursora
	
	- Przesuniêcie wyœwietlacza/kursora
	DB4: 1,
	DB3: 1 - Przesuniêcie wyœwietlacza...
		DB2: 1 - ...w prawo
			0 - ...w lewo
	DB3: 0 - Przesuniêcie kursora...
		DB2: 1 - ...w prawo		(inkrementacja AC)
			0 - ...w lewo		(dekrementacja AC)
			
	- Ustawienie sposobu dzia³ania
	DB5: 1
	DB4: 1 - tryb 8 bitowy	(DB7-DB0)
		0 - tryb 4 bitowy	(DB7:DB4)
	DB3: 1 - U¿ycie dwóch rzêdów do wyœwietlania
		DB2: Wartoœæ bez znaczenia	
	DB3: 0 - Jeden wiersz do wyœwietlania z ustawieniem czcionki...
		DB2: 1 - ...5x10 kropek
			 0 - ...5x8 kropek
			 
	@Remark Ta komenda musi byæ przes³ana na pocz¹tku, poniewa¿ konfigurowany jest tryb dzia³ania wyœwietlacza: 8-bit i 4-bit.
	Tutaj jest u¿ywany tryb 4-bitowy, przez co niektóre ustawienia nie mia³yby skutku.
	W LCD_Init zosta³o to wziête pod uwagê.
	
	- Ustawianie adresów CGRAM:
	DB6:1
	DB5-0:	S³u¿y do stworzenia w³asnych znaków w czcionce 8x5
	
	- Ustawianie adresu DDRAM
	DB7:1
	Ustawienie kursora na konkretne miejsce w wyœwietlaczu
	
	@param[in] commandToWrite komenda do wys³ania
	
*/
void LCD_WriteCommand(unsigned char commandToWrite);

/*
	@brief Wys³anie znaku do wyœwietlenia
	
	Funkcja poprzez TWI wysy³a znak do wyœwietlacza HD44780.
	Do zmiennej tymczasowej, zostaje zapisany stan wysoki RS, stan wysoki 'Enable' (LCD_E) oraz pierwsze 4 bity [7:4] znaku.
	Nastepnie zawartoœæ tej zmiennej jest przes³ana przez TWI do konwertera.
	Potem przes³any jest tylko stan niski 'Enable', by wczeœniej przes³ane dane zosta³y zapisane w wyœwietlaczu.
	Nastepnie zostaje ponownie zapisany stan wysoki 'Enable' oraz pozosta³e 4 bity [3:0] znaku.
	I znowu przes³any jest stan niski 'Enable'
	
	Wys³any znak zostanie wyœwietlony zgodnie z ustawieniami trybu wprowadzania.
	
	Po tym ma miejsce opóŸnienie 50 mikrosekund, wed³ug zaleceñ noty katalogowej wyœwietlacza.
	
	@param[in] dataToWrite znak do wys³ania
*/
void LCD_WriteData(unsigned char dataToWrite);

/*
	@brief Wys³anie ci¹gu znaków do wyœwietlenia
	
	Przechodzi po ³añcuchu znaków wywo³uj¹c funkcjê LCD_WriteData dla ka¿dego znaku.
	
	@param[in] *text wskaŸnik na tablicê znaków do wyœwietlenia
	@note po przekroczeniu zakresu d³ugoœci rzêdu, pozosta³y tekst zostanie wyœwietlony w drugim rzêdzie.
*/
void LCD_WriteText(char *text);

/*
	@brief Ustawienie kursora
	
	Ustawienie kursora na wskazan¹ pozycjê na wyœwietlaczu.
	W jednym rzêdzie jest 16 pozycji,
	adresacja pierwszego rzêdu zaczyna siê od 0x00, a koñczy na 0x0F,
	adresacja drugiego rzêdu zaczyna sie od 0x40, a koñczy na 0x4F.
	
	@param[in] x numer kolumny/pozycji w rzêdzie [0,15].
	@param[in] y numer rzêdu [0,1].
*/
void LCD_GoTo(unsigned char x, unsigned char y);

/*
	@brief Wyczyszczenie ekranu
	
	@note po tej komendzie ma miejsce opóŸnienie 2 ms.
*/
void LCD_Clear(void);

/*
	@brief Skierowanie kursora na pocz¹tek oraz przywrócenie ekranu do stanu przed przesuniêciem
	
	@remark ustawienie te¿ stosuje siê do efektów kursora.
*/
void LCD_Home(void);

/* 
	@brief inicjalizacja ekranu LCD
	
	Wywo³uje szereg funkcji LCD_WriteCommand, z predefiniowanymi ustawieniami. Opis ustawieñ jest w pliku .c
*/
void LCD_Init(void);

#endif//LCD_H
