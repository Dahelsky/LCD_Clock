#ifndef PCF8574_H
#define PCF8574_H

#include <avr/io.h>
#include "twi.h"

/*!
	@file pcf8574.h
	
	@brief Obsluga konwertera TWI - 8bit
	
	Funkcje zapisu i odczytu z konwertera uzywajacego TWI, w wysylaniu i odczycie, zawsze jest najpierw wysylany sygnal startu komunikacji,
	nastepnie w adres urzadzenia oraz bit, ktory decyduje, czy ma miejsce zapis, czy odczyt z urzadzenia.
	[7:1]bity adresu + 1 binarna - odczyt
	[7:1]bity adresu + 0 binarne - zapis
	Po zakonczonej transmijsi wysylany jest sygnal stopu oznaczajacego koniec transmijsi.
	
*/
//#define PCF8574addr				0b01111110		// adres ukladu typu a
#define PCF8574addr				0b01001110		// adres ukladu

/*!
	@brief Odczyt danych
	
	Wysylany jest najpierw przez TWI adres urzadzenia + 1 binarna, a nastepnie odbierany jest ciag 8 bitow, 
		reprezentujacych stan na 8 pinach konwertera.
		
	@returns zwraca odczytane dane
*/
uint8_t PCF8574_ReadPort(void);
/*!
	@brief Zapis danych
	
	Wysylany jest najpierw przez TWI adres urzadzenia + 0 binarne, a nastepnie wysylany jest ciag 8 bitow.

	@param[in] data ciag 8 bitow, ktore maja byc na wyjsciu konwertera.
*/
void PCF8574_WritePort(uint8_t data);

#endif //PCF8574
