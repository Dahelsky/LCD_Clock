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

//#define CHECK

#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

volatile button przyciskGodziny ={BUTTON_GODZ, 0};
volatile button przyciskMinuty ={BUTTON_MIN, 0};
volatile button przyciskSekundy ={BUTTON_SEK, 0};

//volatile uint8_t trybGodz=0;
//volatile uint8_t trybMin=0;
//volatile uint8_t trybSek=0;


uint8_t sprawdzPrzycisk(unsigned char przycisk, uint8_t tryb);
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


FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);

static int czas[] = {2,3,5,9,3,0};

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
	
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
}

int main(void)
{
	
	//przyciski
	BUTTON_DDR &= ~((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //input od przyciskow
	BUTTON_PORT |=  ((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //pullup
	
	
	
	systimer_init(1000);//1sek
	//systimer_init(100);
	
	//timer0
	TCCR0 = (1<<CS02)|(1<<CS00);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 43;
	sei();
	
	
	
	
	TWI_init();
	LCD_Init();
	

	
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);

	
	while (1)
	{
		if(isButtonClicked(przyciskSekundy))
		{
			TCNT1=0;
			czas[5]=0;
			czas[4]=0;
			confirmClick(&przyciskSekundy);
			LCD_Clear();
			
			fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
		}
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
			//przyciskGodziny.state=3;
			confirmClick(&przyciskGodziny);
			LCD_Clear();
			
			fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
		}
	}
	
}
