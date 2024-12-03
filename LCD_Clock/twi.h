#ifndef TWI_H
#define TWI_H

#include <avr/io.h>


/*!
	@file twi.h
	
	@brief Funkcje do obslugi TWI (Two Wire Interface) dla avr
	
	@remark Stworzona z mysla dla aTmega32, na innych mikrokontrolerach moga byc konieczne zmiany
	
	Zawiera szereg funkcji operujacych na rejestrach do obslugi TWI. 
*/


/*!
	@brief ustawianie szybkosci transmijsi danych
*/
void TWI_init(void);

/*!
	@brief Wyslanie sygnalu START na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTA, TWEN zapisywane sa jedynki logiczne.
*/
void TWI_start(void);

/*!
	@brief Wyslanie sygnalu STOP na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTO, TWEN zapisywane sa jedynki logiczne.
*/
void TWI_stop(void);

/*!
	@brief Oczekiwanie na koniec komunikacji
	
	Czeka, az na bicie TWINT rejestru TWCR jest ustawiona jedynka logiczna, oznaczajac koniec transmisji.
*/
void TWI_wait(void);

/*!
	@brief Wyslanie bajtu danych na magistrale TWI
	
	Do rejestru TWDR zapisywany jest bajt danych do przeslania na magistrale TWI
	Nastepnie na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiajac transmisje.
	
	@param[in] data bajt danych do przeslania
*/
void TWI_write(uint8_t data);

/*!
	@brief Odebranie bajtu danych z magistrali TWI
	
	Na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiajac transmisje.
	Nastepnie jest oczekwianie, az zostana przyjete dane w rejestrze TWDR.
	
	@returns odebrany bajt
*/
uint8_t TWI_read(void);

#endif //TWI_H
