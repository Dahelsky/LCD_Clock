#include <util/delay.h>
#include <avr/io.h>
#include "LCD.h"
#include "systimer.h"
#include "buttons.h"
#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>
//#define CHECK

//#define BUTTON_PORT PORTA
//#define BUTTON_DDR	DDRA
//#define BUTTON_PIN	PINA

#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

struct button przyciskGodziny={BUTTON_GODZ,0};
struct button przyciskMinuty={BUTTON_MIN,0};
struct button przyciskSekundy={BUTTON_SEK,0};

volatile uint8_t trybGodz=0;
volatile uint8_t trybMin=0;
volatile uint8_t trybSek=0;

uint8_t sprawdzPrzycisk(uint8_t przycisk, uint8_t tryb);
void zapiszCzas(uint8_t* czas, char* output);
static uint8_t timer0_ovf_counter=0;
ISR(TIMER0_OVF_vect)
{
	if(++timer0_ovf_counter >=2)
	{
		
		//trybGodz=sprawdzPrzycisk(BUTTON_GODZ, trybGodz);
		//trybMin=sprawdzPrzycisk(BUTTON_MIN, trybMin);
		//trybSek=sprawdzPrzycisk(BUTTON_SEK, trybSek);
		przyciskGodziny = checkButton(przyciskGodziny);
		przyciskMinuty = checkButton(przyciskMinuty);
		przyciskSekundy = checkButton(przyciskSekundy);
		timer0_ovf_counter=0;
		TCNT0 = 43;
	}
}

#ifndef CHECK
static uint8_t czas[] = {1,2,0,0,0,0};
#else

static uint8_t czas[] = {1,0,3,0,3,0};
#endif
static char output[9];

void systimer_callback()
{
	#ifdef CHECK
	
	if(++czas[5]>=10)//sekundy - jednosci
	{
		if(++czas[4]>=6) //sekundy - dziesiatki
		{
			czas[4]=0;
		}
		czas[5]=0;
	}
	if(++czas[3]>=10) //minuty - jednosci
	{
		if(++czas[2]>=6) //minuty - dziesiatki
		{
			//godziny jednosci
			
			czas[2]=0;
		}
		czas[3]=0;
	}
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
	
	#else
	
	if(++czas[5]>=10)//sekundy jednosci
	{
		if(++czas[4]>=6) //sekundy dziesiatki
		{
			if(++czas[3]>=10) // minuty jednosci
			{
				if(++czas[2]>=6) //minuty dziesiatki
				{
					//godziny
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
	#endif
	LCD_Clear();
	zapiszCzas(czas, output);
	LCD_Text(output);
}

int main(void)
{
	
	//przyciski
	//BUTTON_DDR &= ~((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //input od przyciskow
	//BUTTON_PORT |=  ((1<<BUTTON_GODZ)|(1<<BUTTON_MIN)|(1<<BUTTON_SEK)); //pullup
	
	#ifndef CHECK
	systimer_init(1000);//1sek
	#else
	systimer_init(500);
	#endif
	//timer0
	TCCR0 = (1<<CS02)|(1<<CS00);
	TIMSK |= (1<<TOIE0);
	TCNT0 = 43;
	sei();
	
	buttonInit(BUTTON_GODZ);
	buttonInit(BUTTON_MIN);
	buttonInit(BUTTON_SEK);
	LCD_Init();
	
	zapiszCzas(czas, output);
	LCD_Text(output);
	
	//printf(var2);
	while (1)
	{
		//0-1, 2-3, 4-5	
		//if(trybSek==2)
		if(przyciskSekundy.state==2)
		{
			TCNT1=0;
			//trybSek=3;
			przyciskSekundy.state=3;
			czas[5]=0;
			czas[4]=0;
			LCD_Clear();
			zapiszCzas(czas, output);
			LCD_Text(output);
		}
		
		//if(trybMin==2)
		if(przyciskMinuty.state==2)
		{
			//trybMin=3;
			przyciskMinuty.state = 3;
			if(++czas[3]>=10)
			{
				if(++czas[2]>=6)
				{
					czas[2]=0;
				}
				czas[3]=0;
			}
			LCD_Clear();
			zapiszCzas(czas, output);
			LCD_Text(output);
		}
		//if(trybGodz==2)
		if(przyciskGodziny.state==2)
		{
			//trybGodz=3;
			przyciskGodziny.state=3;
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
			zapiszCzas(czas, output);
			LCD_Text(output);
		}
	}
}
uint8_t sprawdzPrzycisk(uint8_t przycisk, uint8_t tryb)
{
	if(!(BUTTON_PIN & (1<<przycisk)))
	{
		switch(tryb)
		{
			case 0:
				return 1;
			case 1:
				return 2;
		}
	}
	else
	{
		switch(tryb)
		{
			case 3:
				return 4;
			case 4:
				return 0;
		}
	}
	return tryb;
}

void zapiszCzas(uint8_t* czas, char* output)
{
	output[0]=czas[0]+'0';
	output[1]=czas[1]+'0';
	output[2]=':';
	output[3]=czas[2]+'0';
	output[4]=czas[3]+'0';
	output[5]=':';
	output[6]=czas[4]+'0';
	output[7]=czas[5]+'0';
	output[8]='\0';
}