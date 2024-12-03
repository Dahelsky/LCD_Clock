#include "systimer.h"

uint8_t systimer_init(uint16_t systick)
{
	uint64_t _OCR1A;
	uint8_t index = 0;
	
	uint16_t prescaler_div[5] = {1, 8, 64, 256, 1024};
	
	do 
	{
		_OCR1A = ((F_CPU / prescaler_div[index]) * systick)/1000ULL;
	} while ((_OCR1A > 65536) && (index++ < 5));
	
	uint8_t error_code = 0;
	if(index >= 5)
		error_code = SYSTIMER_ERROR_2;
	else if((((F_CPU / prescaler_div[index]) * systick)/1000ULL) !=0)
		error_code = SYSTIMER_ERROR_1;
	else
		error_code = SYSTIMER_NO_ERROR;
		
	if(error_code < SYSTIMER_ERROR_2)
	{
		OCR1A = _OCR1A;
		TIMSK |= (1 << OCIE1A);
		TCCR1B = (1 << WGM12) | (index + 1);
	}
		
	return error_code;
}

ISR(TIMER1_COMPA_vect) //przerwanie
{
	systimer_callback();
}

void systimer_callback(void)
{
	
}