#ifndef TWI_H
#define TWI_H

#include <avr/io.h>


/*
	@brief Funkcje do obs³ugi TWI (Two Wire Interface) dla avr
	
	@remark Stworzona z myœl¹ dla ATmega32, na innych mikrokontrolerach mog¹ byæ konieczne zmiany
	
	Zawiera szereg funkcji operuj¹cych na rejestrach do obs³ugi TWI. 
*/


/*
	@brief ustawianie szybkoœci transmijsi danych
*/
void TWI_init(void);

/*
	@brief Wys³anie sygna³u START na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTA, TWEN zapisywane s¹ jedynki logiczne.
*/
void TWI_start(void);

/*
	@brief Wys³anie sygna³u STOP na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTO, TWEN zapisywane s¹ jedynki logiczne.
*/
void TWI_stop(void);

/*
	@brief Oczekiwanie na koniec komunikacji
	
	Czeka, a¿ na bicie TWINT rejestru TWCR jest ustawiona jedynka logiczna, oznaczaj¹c koniec transmisji.
*/
void TWI_wait(void);

/*
	@brief Wys³anie bajtu danych na magistralê TWI
	
	Do rejestru TWDR zapisywany jest bajt danych do przes³ania na magistralê TWI
	Nastêpnie na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiaj¹c transmisjê.
	
	@param[in] data bajt danych do przes³ania
*/
void TWI_write(uint8_t data);

/*
	@brief Odebranie bajtu danych z magistrali TWI
	
	Na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiaj¹c transmisjê.
	Nastêpnie jest oczekwianie, a¿ zostan¹ przyjête dane w rejestrze TWDR.
	
	@returns odebrany bajt
*/
uint8_t TWI_read(void);

#endif //TWI_H
