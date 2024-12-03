#include "twi.h"
#include "lcd.h"
#include "systimer.h"
#include "button.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

// przyciski
#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

//inicjalizacja struktur przycisków dla czasu
button przyciskGodziny={BUTTON_GODZ,0};
button przyciskMinuty={BUTTON_MIN,0};
button przyciskSekundy={BUTTON_SEK,0};


//zliczanie przerwañ licznika 0
volatile uint8_t timer0_ovf_counter=0;

//eliminacja drgañ
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

void wyswietlCzas(uint8_t* czas); // wyœwietlanie czasu na wyœwietlaczu LCD
FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);


// 0,1 - godziny
// 2,3 - minuty
// 4,5 - sekundy
static uint8_t  czas[] = {1,0,3,0,3,0};

void systimer_callback()
{
	//sekundy
	if(++czas[5]>=10)//jednoœci
	{
		if(++czas[4]>=6) //dziesi¹tki
		{
			//minuty
			if(++czas[3]>=10) //jednoœci
			{
				if(++czas[2]>=6) //dziesiatki
				{
					//godziny
					// przedzia³: 0-19
					if((czas[0]<2) && (++czas[1]>=10))
					{
						++czas[0];
						czas[1]=0;
					}
					// przedzia³: 20-24
					else if((czas[0]==2) && (++czas[1]>=4))
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
	//inicjalizacja
	
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
			//resetowanie systimera
			TCNT1=0;
			confirmClick(&przyciskGodziny);
			//zerowanie sekund
			czas[5]=0;
			czas[4]=0;
			LCD_Clear();
			wyswietlCzas(czas);
		}
		if(isButtonClicked(przyciskMinuty));
		{
			confirmClick(&przyciskGodziny);
			//inkrementowanie minut
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
			//inkrementowanie godzin
			if((czas[0]<2) && (++czas[1]>=10)) // przedzia³: 0-19
			{
				++czas[0];
				czas[1]=0;
			}
			else if((czas[0]==2) && (++czas[1]>=4)) // przedzia³: 20-24
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