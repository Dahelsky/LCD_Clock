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
	@brief Obs³uga przycisków
	
	@attention Zak³ada siê, ¿e przyciski s¹ po³¹czone do portu A oraz do rezystora pull-up, czyli w stanie spoczynku na wejœciu bêdzie stan wysoki.
*/


/*
	@brief Struktura opisuj¹ca przycisk
	
	Sk³ada siê z dwóch pól:
	@a pin opisuj¹cy numer pinu portu A
	@a state opisuj¹cy stan przycisku, ma wartoœci od 0 do 4:
	0 - przycisk nie klikniêty
	1 - przycisk klikniêty
	2 - potwierdzenie klikniêcia przycisku
	3 - potwierdzenie wykonania procesu zwi¹zanego z przyciskiem 
	4 - przycisk zwolniony
	
	@brief przy inicjalizacji struktury, wartoœæ @state powinna byæ równa 0, tak jak na przyk³adzie poni¿ej.
	Przyk³ad inicjalizacji struktury przycisku pod³¹czonego do pinu 1 portu A:
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
	
	Ustawia wartoœæ konkretnego bitu DDRA na 0 (input) oraz bitu PORTA (pullup)
	
	@param[in] pin numer pinu (1-8)
*/
void buttonInit(uint8_t pin);

/*
	@brief Aktualizacja stanu przycisku
	
	Funkcja sprawdza stan na pinie przypisanym do przycisku,
	jeœli jest niski to oznacza, ¿e jest klikniêty. 
	Gdy klikniêty to sprawdza @a state struktury, gdy 0 to go ustawia na 1, a gdy 1 to na 2.
	Jeœli przycisk nie jest klikniêty to równie¿ sprawdza @a state, jesli jest 3 to ustawia na 4, gdy jest 4 to ustawia na 0
	
	W przypadku braku zmian, zwraca strukturê bez zmiany,
	
	@param[in] checkedButton sprawdzany przycisk
	@returns zwracana struktura z nowymi wartoœciami.
*/
button checkButton(button checkedButton);

/*
	@brief Sprawdza czy przycisk by³ dalej klikniêty po drganiu styków
	
	Po prostu spradza czy wartoœæ @a button.state ma wartoœæ 2.
	
	@param[in] checkedButton sprawdzany przycisk
	@returns 1 gdy @a button.state ma wartosæ 2, w przeciwnym wypadku zwraca 0
*/
uint8_t isButtonClicked(button checkedButton);

/*
	@brief Potwierdzenie klikniêcia przycisku
	
	Ustawia wartoœæ @a button.state na 3, potwierdzaj¹c obs³ugê efektu klikniêcia przycisku.
	
	@param[in] wskaŸnik na przycisk
*/
void confirmClick(button* Button);

#endif /* BUTTONS_H_ */