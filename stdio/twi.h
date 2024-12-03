#ifndef TWI_H
#define TWI_H

#include <avr/io.h>


/*
	@brief Funkcje do obs�ugi TWI (Two Wire Interface) dla avr
	
	@remark Stworzona z my�l� dla ATmega32, na innych mikrokontrolerach mog� by� konieczne zmiany
	
	Zawiera szereg funkcji operuj�cych na rejestrach do obs�ugi TWI. 
*/


/*
	@brief ustawianie szybko�ci transmijsi danych
*/
void TWI_init(void);

/*
	@brief Wys�anie sygna�u START na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTA, TWEN zapisywane s� jedynki logiczne.
*/
void TWI_start(void);

/*
	@brief Wys�anie sygna�u STOP na magistrali TWI
	
	W rejestrze TWCR na bitach TWINT, TWSTO, TWEN zapisywane s� jedynki logiczne.
*/
void TWI_stop(void);

/*
	@brief Oczekiwanie na koniec komunikacji
	
	Czeka, a� na bicie TWINT rejestru TWCR jest ustawiona jedynka logiczna, oznaczaj�c koniec transmisji.
*/
void TWI_wait(void);

/*
	@brief Wys�anie bajtu danych na magistral� TWI
	
	Do rejestru TWDR zapisywany jest bajt danych do przes�ania na magistral� TWI
	Nast�pnie na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiaj�c transmisj�.
	
	@param[in] data bajt danych do przes�ania
*/
void TWI_write(uint8_t data);

/*
	@brief Odebranie bajtu danych z magistrali TWI
	
	Na rejestr TWCR na bitach TWINT i TWEN zapisywana jest logiczna jedynka uruchamiaj�c transmisj�.
	Nast�pnie jest oczekwianie, a� zostan� przyj�te dane w rejestrze TWDR.
	
	@returns odebrany bajt
*/
uint8_t TWI_read(void);

#endif //TWI_H
