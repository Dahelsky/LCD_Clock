#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#include <stdint.h>
#include <avr/interrupt.h>

#define SYSTIMER_NO_ERROR	0
#define SYSTIMER_ERROR_1	1
#define SYSTIMER_ERROR_2	2

#define F_CPU 16000000UL

uint8_t systimer_init(uint16_t systick); // ms
void systimer_callback(void) __attribute__((weak));



#endif /* SYSTIMER_H_ */