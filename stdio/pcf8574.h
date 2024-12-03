#ifndef PCF8574_H
#define PCF8574_H

#include <avr/io.h>
#include "twi.h"

/*
	@brief Obs�uga konwertera TWI - 8bit
	
	Funkcje zapisu i odczytu z konwertera u�ywaj�cego TWI, w wysy�aniu i odczycie, zawsze jest najpierw wysy�any sygna� startu komunikacji,
	nastepnie w adres urz�dzenia oraz bit, kt�ry decyduje, czy ma miejsce zapis, czy odczyt z urz�dzenia.
	[7:1]bity adresu + 1 binarna - odczyt
	[7:1]bity adresu + 0 binarne - zapis
	Po zako�czonej transmijsi wysy�any jest sygna� stopu oznaczaj�cego koniec transmijsi.
	
*/
//#define PCF8574Addr				0b01111110		// adres ukladu typu A
#define PCF8574Addr				0b01001110		// adres ukladu

/*
	@brief Odczyt danych
	
	Wysy�any jest najpierw przez TWI adres urz�dzenia + 1 binarna, a nast�pnie odbierany jest ci�g 8 bit�w, 
		reprezentuj�cych stan na 8 pinach konwertera.
		
	@returns zwraca odczytane dane
*/
uint8_t PCF8574_ReadPort(void);
/*
	@brief Zapis danych
	
	Wysy�any jest najpierw przez TWI adres urz�dzenia + 0 binarne, a nast�pnie wysy�any jest ci�g 8 bit�w.

	@param[in] data ci�g 8 bit�w, kt�re maj� by� na wyj�ciu konwertera.
*/
void PCF8574_WritePort(uint8_t data);

#endif //PCF8574
