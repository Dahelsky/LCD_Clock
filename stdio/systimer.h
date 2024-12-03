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
	
	U�ywany jest Timer 1, kt�ry jest 16 bitowy.
	na podstawie F_CPU i podanego argumentu ustawia preskaler licznika oraz warto��, do kt�rej ma by� por�wnywany rejestr licznika.
	
	@remark s� zakresy czasu dla kt�rych funckja mo�e nie dzia�a� poprawnie, ale obecnie trzeba si� samemu przekona� o nich, wtedy pasuje kombinowa� z zliaczaniem przerwa�, aby osi�gn�� wi�ksze interwa�y czasowe
	@remark funkcja nie w��cza przerwa� poprzez sei(), nale�y to doda� do programu
	@param[in] systick czas w ms, po kt�rym ma by� przerwanie.
	@returns kod b�edu
*/
uint8_t systimer_init(uint16_t systick); // ms

/*
	@brief funkcja wykonywana w momencie przerwania
	
	Pusta funkcja, kt�ra jest wywo�ywana, parametr 'weak' pozwala na zast�pienie jej implementacji.
	
*/
void systimer_callback(void) __attribute__((weak));



#endif /* SYSTIMER_H_ */