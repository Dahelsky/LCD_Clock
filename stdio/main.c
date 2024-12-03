#include "main.h"
#include "pcf8574.h"
#include "twi.h"
#include "lcd.h"
#include "systimer.h"
#include "button.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

//#define CHECK

// przyciski
#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

button przyciskGodziny={BUTTON_GODZ,0};
button przyciskMinuty={BUTTON_MIN,0};
button przyciskSekundy={BUTTON_SEK,0};


volatile uint8_t timer0_ovf_counter=0;

ISR(TIMER0_OVF_vect)
{
	if(++timer0_ovf_counter >=2)
	{
		przyciskGodziny = checkButton(przyciskGodziny);
		przyciskMinuty = checkButton(przyciskMinuty);
		przyciskSekundy = checkButton(przyciskSekundy);
		timer0_ovf_counter=0;
		TCNT0 = 43;
	}
}

void wyswietlCzas(uint8_t* czas);
FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);


#ifndef CHECK
static uint8_t czas[] = {2,3,5,9,3,0};
#else

static uint8_t  czas[] = {1,0,3,0,3,0};
#endif

void systimer_callback()
{
	if(++czas[5]>=10)//sek jednosci
	{
		if(++czas[4]>=6) //sek dziesi
		{
			if(++czas[3]>=10) // min jednosci
			{
				if(++czas[2]>=6) //min dziesiatki
				{
					//godziny jednosci
					if((czas[0]<2) && (++czas[1]>=10)) // godz: 0-19
					{
						++czas[0];
						czas[1]=0;
					}
					else if((czas[0]==2) && (++czas[1]>=4)) // godz: 20-24
					{
						czas[1]=0;
						czas[0]=0;
					}
					czas[2]=0;
				}
				czas[3]=0;
			}
			czas[4]=0;
		}
		czas[5]=0;
	}
	
	LCD_Clear();
	
	wyswietlCzas(czas);
}

int main(void)
{
	
	systimer_init(1000);//1sek
	
	//timer0
	TCCR0 = (1<<CS02)|(1<<CS00);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 43;
	sei();
	
	TWI_init();
	LCD_Init();
	
	wyswietlCzas(czas);

	while (1) 
	{
		if(isButtonClicked(przyciskSekundy))
		{
			TCNT1=0;
			confirmClick(&przyciskGodziny);
			czas[5]=0;
			czas[4]=0;
			LCD_Clear();
			wyswietlCzas(czas);
		}
		if(isButtonClicked(przyciskMinuty));
		{
			confirmClick(&przyciskGodziny);
			if(++czas[3]>=10)
			{
				if(++czas[2]>=6)
				{
					czas[2]=0;
				}
				czas[3]=0;
				LCD_Clear();
				wyswietlCzas(czas);
			}
		}
		
		if(isButtonClicked(przyciskGodziny))
		{
			confirmClick(&przyciskGodziny);
			if((czas[0]<2) && (++czas[1]>=10)) // godz: 0-19
			{
				++czas[0];
				czas[1]=0;
			}
			else if((czas[0]==2) && (++czas[1]>=4)) // godz: 20-24
			{
				czas[1]=0;
				czas[0]=0;
			}
			LCD_Clear();
			wyswietlCzas(czas);
		}
	}
}

void wyswietlCzas(uint8_t* czas)
{
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
}