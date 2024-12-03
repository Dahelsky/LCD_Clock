#ifndef SYSTIMER_H_
#define SYSTIMER_H_

#include <stdint.h>
#include <avr/interrupt.h>

#define SYSTIMER_NO_ERROR	0
#define SYSTIMER_ERROR_1	1
#define SYSTIMER_ERROR_2	2

#define F_CPU 16000000UL


/**
  *	@brief inicjalizacja licznika
	
	U¿ywany jest Timer 1, który jest 16 bitowy.
	na podstawie F_CPU i podanego argumentu ustawia preskaler licznika oraz wartoœæ, do której ma byæ porównywany rejestr licznika.
	
	@remark s¹ zakresy czasu dla których funckja mo¿e nie dzia³aæ poprawnie, ale obecnie trzeba siê samemu przekonaæ o nich, wtedy pasuje kombinowaæ z zliaczaniem przerwañ, aby osi¹gn¹æ wiêksze interwa³y czasowe
	@remark funkcja nie w³¹cza przerwañ poprzez sei(), nale¿y to dodaæ do programu
	@param[in] systick czas w ms, po którym ma byæ przerwanie.
	@returns kod b³edu
*/
uint8_t systimer_init(uint16_t systick); // ms

/*
	@brief funkcja wykonywana w momencie przerwania
	
	Pusta funkcja, która jest wywo³ywana, parametr 'weak' pozwala na zast¹pienie jej implementacji.
	
*/
void systimer_callback(void) __attribute__((weak));



#endif /* SYSTIMER_H_ */