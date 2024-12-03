#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#define BUTTON_PORT PORTA
#define BUTTON_DDR	DDRA
#define BUTTON_PIN	PINA

struct button {
	uint8_t pin;
	uint8_t state;
};
void buttonInit(uint8_t pin);
struct button checkButton(struct button checkedButton);


#endif /* BUTTONS_H_ */