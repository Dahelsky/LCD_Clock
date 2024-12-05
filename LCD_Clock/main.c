#include "main.h"
#include <util/delay.h>
#include <avr/io.h>
#include "button.h"
#include "pcf8574.h"
#include "twi.h"
#include "lcd.h"
#include "systimer.h"

#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

volatile button przyciskGodziny ={BUTTON_GODZ, 0};
volatile button przyciskMinuty ={BUTTON_MIN, 0};
volatile button przyciskSekundy ={BUTTON_SEK, 0};

uint8_t sprawdzPrzycisk(unsigned char przycisk, uint8_t tryb);

volatile uint8_t timer0_ovf_counter=0;

//obsluga przerwania licznika 0 - przepelnienie
ISR(TIMER0_OVF_vect)
{
	if(++timer0_ovf_counter >=2)
	{
		//sprawdzanie przyciskow
		
		przyciskGodziny = checkButton(przyciskGodziny);
		przyciskMinuty = checkButton(przyciskMinuty);
		przyciskSekundy = checkButton(przyciskSekundy);
		
		//reset licznika przepelnien
		timer0_ovf_counter=0;
		TCNT0 = 43;
	}
}

//przypisanie wyswietlania tesktu do funkcji wyswietlania na LCD 
//(tu funkcja fprintf teraz bedzie wysylac tekst na wyswietlacz)
FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);

//tablica z czasem
//[0,1] - godziny
//[2,3] - minuty
//[4,5] - godziny
static int czas[] = {2,3,5,9,3,0};

void systimer_callback()
{
	//zegar
	
	//inkrementacja sekund
	if(++czas[5]>=10)//jednosci
	{
		if(++czas[4]>=6) //dziesiatki
		{
			//inkrementacja minut
			if(++czas[3]>=10) //jednosci
			{
				if(++czas[2]>=6) //dziesiatki
				{
					//inkrementacja godzin
					if((czas[0]<2) && (++czas[1]>=10)) // przedzial: 0-19
					{
						++czas[0];
						czas[1]=0;
					}
					else if((czas[0]==2) && (++czas[1]>=4)) // przedzial: 20-24
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
	
	//wyswietlenie czasu
	LCD_Clear();
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
}

int main(void)
{
	//inicjalizacja
	
	//przyciski
	BUTTON_DDR &= ~((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //input od przyciskow
	BUTTON_PORT |=  ((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //pullup
	
	//Liczniki:
	
	//Zegar (licznik 1)
	systimer_init(1000);//1sek
	
	//Eliminacja drgan stykow (licznik 0)
	TCCR0 = (1<<CS02)|(1<<CS00);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 43;
	sei();
	
	//Wyswietlacz + konwerter TWI-8 bit
	TWI_init();
	LCD_Init();
	
	//wyswietlenie czasu
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
	
	while (1)
	{
		//sprawdzenie czy przyciski sa klikniete
		
		//przycisk zerowania sekund
		if(isButtonClicked(przyciskSekundy))
		{
			TCNT1=0;
			czas[5]=0;
			czas[4]=0;
			confirmClick(&przyciskSekundy);
			LCD_Clear();
			
			fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
		}
		
		//przycisk inkrementacji minut
		if(isButtonClicked(przyciskMinuty))
		{
			if(++czas[3]>=10)
			{
				if(++czas[2]>=6)
				{
					czas[2]=0;
				}
				czas[3]=0;
		}
			confirmClick(&przyciskMinuty);
			LCD_Clear();
			
			fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
		}
		
		//przycisk inkrementacji godzin
		if(isButtonClicked(przyciskGodziny))
		{
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
			
			confirmClick(&przyciskGodziny);
			LCD_Clear();
			
			fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
		}
	}
	
}
