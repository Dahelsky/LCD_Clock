#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#define BUTTON_PORT PORTA
#define BUTTON_DDR	DDRA
#define BUTTON_PIN	PINA

#
/*
	@brief Obs�uga przycisk�w
	
	@attention Zak�ada si�, �e przyciski s� po��czone do portu A oraz do rezystora pull-up, czyli w stanie spoczynku na wej�ciu b�dzie stan wysoki.
*/


/*
	@brief Struktura opisuj�ca przycisk
	
	Sk�ada si� z dw�ch p�l:
	@a pin opisuj�cy numer pinu portu A
	@a state opisuj�cy stan przycisku, ma warto�ci od 0 do 4:
	0 - przycisk nie klikni�ty
	1 - przycisk klikni�ty
	2 - potwierdzenie klikni�cia przycisku
	3 - potwierdzenie wykonania procesu zwi�zanego z przyciskiem 
	4 - przycisk zwolniony
	
	@brief przy inicjalizacji struktury, warto�� @state powinna by� r�wna 0, tak jak na przyk�adzie poni�ej.
	Przyk�ad inicjalizacji struktury przycisku pod��czonego do pinu 1 portu A:
	<pre>
	button przycisk={1, 0}
	</pre>
	
*/
typedef struct button {
	uint8_t pin;	///< numer pinu
	uint8_t state;	///< stan przycisku
}button;

/*
	@brief inicjalizacja portu A dla konkretnego przycisku
	
	Ustawia warto�� konkretnego bitu DDRA na 0 (input) oraz bitu PORTA (pullup)
	
	@param[in] pin numer pinu (1-8)
*/
void buttonInit(uint8_t pin);

/*
	@brief Aktualizacja stanu przycisku
	
	Funkcja sprawdza stan na pinie przypisanym do przycisku,
	je�li jest niski to oznacza, �e jest klikni�ty. 
	Gdy klikni�ty to sprawdza @a state struktury, gdy 0 to go ustawia na 1, a gdy 1 to na 2.
	Je�li przycisk nie jest klikni�ty to r�wnie� sprawdza @a state, jesli jest 3 to ustawia na 4, gdy jest 4 to ustawia na 0
	
	W przypadku braku zmian, zwraca struktur� bez zmiany,
	
	@param[in] checkedButton sprawdzany przycisk
	@returns zwracana struktura z nowymi warto�ciami.
*/
button checkButton(button checkedButton);

/*
	@brief Sprawdza czy przycisk by� dalej klikni�ty po drganiu styk�w
	
	Po prostu spradza czy warto�� @a button.state ma warto�� 2.
	
	@param[in] checkedButton sprawdzany przycisk
	@returns 1 gdy @a button.state ma wartos� 2, w przeciwnym wypadku zwraca 0
*/
uint8_t isButtonClicked(button checkedButton);

/*
	@brief Potwierdzenie klikni�cia przycisku
	
	Ustawia warto�� @a button.state na 3, potwierdzaj�c obs�ug� efektu klikni�cia przycisku.
	
	@param[in] wska�nik na przycisk
*/
void confirmClick(button* Button);

#endif /* BUTTONS_H_ */