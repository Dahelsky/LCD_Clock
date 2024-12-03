#ifndef PCF8574_H
#define PCF8574_H

#include <avr/io.h>
#include "twi.h"

/*
	@brief Obs³uga konwertera TWI - 8bit
	
	Funkcje zapisu i odczytu z konwertera u¿ywaj¹cego TWI, w wysy³aniu i odczycie, zawsze jest najpierw wysy³any sygna³ startu komunikacji,
	nastepnie w adres urz¹dzenia oraz bit, który decyduje, czy ma miejsce zapis, czy odczyt z urz¹dzenia.
	[7:1]bity adresu + 1 binarna - odczyt
	[7:1]bity adresu + 0 binarne - zapis
	Po zakoñczonej transmijsi wysy³any jest sygna³ stopu oznaczaj¹cego koniec transmijsi.
	
*/
//#define PCF8574Addr				0b01111110		// adres ukladu typu A
#define PCF8574Addr				0b01001110		// adres ukladu

/*
	@brief Odczyt danych
	
	Wysy³any jest najpierw przez TWI adres urz¹dzenia + 1 binarna, a nastêpnie odbierany jest ci¹g 8 bitów, 
		reprezentuj¹cych stan na 8 pinach konwertera.
		
	@returns zwraca odczytane dane
*/
uint8_t PCF8574_ReadPort(void);
/*
	@brief Zapis danych
	
	Wysy³any jest najpierw przez TWI adres urz¹dzenia + 0 binarne, a nastêpnie wysy³any jest ci¹g 8 bitów.

	@param[in] data ci¹g 8 bitów, które maj¹ byæ na wyjœciu konwertera.
*/
void PCF8574_WritePort(uint8_t data);

#endif //PCF8574
