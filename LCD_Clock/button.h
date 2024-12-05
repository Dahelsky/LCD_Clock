#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#define BUTTON_PORT PORTA
#define BUTTON_DDR	DDRA
#define BUTTON_PIN	PINA

#
/*!
	@file button.h

	@brief Obsluga przyciskow
	
	@remark Zalozono, ze przyciski sa polaczone do portu a oraz do rezystora pull-up, czyli w stanie spoczynku na wejsciu bedzie stan wysoki.
*/


/*!
	@brief Struktura opisujaca przycisk
	
	Sklada sie z dwoch pol:
	@a pin opisujacy numer pinu portu a
	@a state opisujacy stan przycisku, ma wartosci od 0 do 4:
	0 - przycisk nie klikniety
	1 - przycisk klikniety
	2 - potwierdzenie klikniecia przycisku
	3 - potwierdzenie wykonania procesu zwiazanego z przyciskiem 
	4 - przycisk zwolniony
	
	@brief przy inicjalizacji struktury, wartosc @state powinna byc rowna 0, tak jak na przykladzie ponizej.
	Przyklad inicjalizacji struktury przycisku podlaczonego do pinu 1 portu a:
	<pre>
	button przycisk={1, 0}
	</pre>
	
*/

typedef struct button {
	uint8_t pin;	///< numer pinu
	uint8_t state;	///< stan przycisku
}button;

/*!
	@brief aktualizacja stanu przycisku
	
	Funkcja sprawdza stan na pinie przypisanym do przycisku,
	jesli jest niski to oznacza, ze jest klikniety. 
	Gdy klikniety to sprawdza @a state struktury, gdy 0 to go ustawia na 1, a gdy 1 to na 2.
	Jesli przycisk nie jest klikniety to rowniez sprawdza @a state, jesli jest 3 to ustawia na 4, gdy jest 4 to ustawia na 0
	
	W przypadku braku zmian, zwraca strukture bez zmiany,
	
	@param[in] checkedButton sprawdzany przycisk
	@returns zwracana struktura z nowymi wartosciami.
*/
button checkButton(button checkedButton);

/*!
	@brief Sprawdza czy przycisk byl dalej klikniety po drganiu stykow
	
	Po prostu spradza czy wartosc @a button.state ma wartosc 2.
	
	@param[in] checkedButton sprawdzany przycisk
	@returns 1 gdy @a button.state ma wartosc 2, w przeciwnym wypadku zwraca 0
*/
uint8_t isButtonClicked(button checkedButton);

/*!
	@brief Potwierdzenie klikniecia przycisku
	
	Ustawia wartosc @a button.state na 3, potwierdzajac obsluge efektu klikniecia przycisku.
	
	@param[in] wskaznik na przycisk
*/
void confirmClick(button* Button);

#endif /* BUTTONS_H_ */