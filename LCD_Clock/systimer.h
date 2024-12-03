#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#include <stdint.h>
#include <avr/interrupt.h>

#define SYSTIMER_NO_ERROR	0
#define SYSTIMER_ERROR_1	1
#define SYSTIMER_ERROR_2	2

#define F_CPU 16000000UL


/*!
	@file systimer.h
	
	@brief Obsluga licznika 1
*/



/*!
  	@brief inicjalizacja licznika
	
	Uzywany jest Timer 1, ktory jest 16 bitowy.
	na podstawie F_CPU i podanego argumentu ustawia preskaler licznika oraz wartosc, do ktorej ma byc porownywany rejestr licznika.
	
	@remark sa zakresy czasu dla ktorych funckja moze nie dzialac poprawnie, ale obecnie trzeba sie samemu przekonac o nich, wtedy pasuje kombinowac z zliaczaniem przerwan, aby osiagnac wieksze interwaly czasowe
	@remark funkcja nie wlacza przerwan poprzez sei(), nalezy to dodac do programu
	@param[in] systick czas w ms, po ktorym ma byc przerwanie.
	@returns kod bledu
*/
uint8_t systimer_init(uint16_t systick); // ms

/*!
	@brief funkcja wykonywana w momencie przerwania
	
	Pusta funkcja, ktora jest wywolywana, parametr 'weak' pozwala na zastapienie jej implementacji.
	
*/
void systimer_callback(void) __attribute__((weak));



#endif /* SYSTIMER_H_ */