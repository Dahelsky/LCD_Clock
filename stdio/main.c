#include "main.h"
#include <util/delay.h>
#include <avr/io.h>
#include "pcf8574.h"
#include "twi.h"
#include "lcd.h"
#include "systimer.h"

#include <stdio.h>
#include <avr/pgmspace.h>
#include <stdint.h>

//#define CHECK

//przycisk deodbijacz
#define BUTTON_PORT PORTA
#define BUTTON_DDR	DDRA
#define BUTTON_PIN	PINA


#define BUTTON_GODZ 2
#define BUTTON_MIN 1
#define BUTTON_SEK 0

volatile uint8_t trybGodz=0;
volatile uint8_t trybMin=0;
volatile uint8_t trybSek=0;

uint8_t sprawdzPrzycisk(unsigned char przycisk, uint8_t tryb);
volatile uint8_t timer0_ovf_counter=0;
ISR(TIMER0_OVF_vect)
{
	if(++timer0_ovf_counter >=2)
	{
		if(!(BUTTON_PIN & (1<<BUTTON_GODZ)))
		{
			switch(trybGodz)
			{
				case 0:
				trybGodz= 1;
				break;
				case 1:
				trybGodz= 2;
				break;
			}
		}
		else
		{
			switch(trybGodz)
			{
				case 3:
				trybGodz= 4;
				break;
				case 4:
				trybGodz= 0;
				break;
			}
		}
		if(!(BUTTON_PIN & (1<<BUTTON_MIN)))
		{
			switch(trybMin)
			{
				case 0:
				trybMin= 1;
				break;
				case 1:
				trybMin= 2;
				break;
			}
		}
		else
		{
			switch(trybMin)
			{
				case 3:
				trybMin= 4;
				break;
				case 4:
				trybMin= 0;
				break;
			}
		}
		if(!(BUTTON_PIN & (1<<BUTTON_SEK)))
		{
			switch(trybSek)
			{
				case 0:
				trybSek= 1;
				break;
				case 1:
				trybSek= 2;
				break;
			}
		}
		else
		{
			switch(trybSek)
			{
				case 3:
				trybSek= 4;
				break;
				case 4:
				trybSek= 0;
				break;
			}
		}
		timer0_ovf_counter=0;
		TCNT0 = 43;
	}
	
	
		//trybGodz = sprawdzPrzycisk(BUTTON_GODZ, trybGodz);
		//trybMin = sprawdzPrzycisk(BUTTON_GODZ, trybMin);
		//trybSek = sprawdzPrzycisk(BUTTON_GODZ, trybSek);
	
	
}



FILE LCDstdout = FDEV_SETUP_STREAM((void*)LCD_WriteData, NULL, _FDEV_SETUP_WRITE);




#ifndef CHECK
static int czas[] = {2,3,5,9,3,0};
#else

static int czas[] = {1,0,3,0,3,0};
#endif

void systimer_callback()
{
	#ifdef CHECK
	if(++czas[5]>=10)//sek jednosci
	{
		if(++czas[4]>=6) //sek dziesi
		{
			
			czas[4]=0;
		}
		czas[5]=0;
	}
	if(++czas[3]>=10) // min jednosci
	{
		if(++czas[2]>=6) //min dziesiatki
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
	#endif
	
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
	

	//stdout = fdevopen((void *)LCD_WriteData, NULL);
	//stdout = &LCDstdout;
	
	//uint8_t var1 = 125;
	//float var2 = 12.63;
	
	fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);

	
	
	//printf(czas[2]);
	//LCD_GoTo(0,1); //druga linia
	//printf("have some oats");
	
	
	//printf(var2);
	while (1) 
  {
	  if(trybSek==2)
	  {
		  czas[5]=0;
		  czas[4]=0;
		  trybSek=3;
		  LCD_Clear();
		  
		  fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
	  }
	  if(trybMin==2)
	  {
		  if(++czas[3]>=10)
		  {
			if(++czas[2]>=6)
			{
				czas[2]=0;
			}
			czas[3]=0;
		  }
		  trybMin=3;
		  LCD_Clear();
		  
		  fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
	  }
	  if(trybGodz==2)
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
		  trybGodz=3;
		  LCD_Clear();
		  
		  fprintf_P(&LCDstdout, PSTR("%i%i:%i%i:%i%i"), czas[0],czas[1],czas[2],czas[3],czas[4],czas[5]);
	  }
  }
  
}

uint8_t sprawdzPrzycisk(unsigned char przycisk, uint8_t tryb)
{
	if(!(BUTTON_PIN & (1<<przycisk)))
	{
		switch(tryb)
		{
			case 0:
				return 1;
			break;
			case 1:
				return 2;
			break;
		}
	}
	else
	{
		switch(tryb)
		{
			case 3:
				return 4;
			break;
			case 4:
				return 0;
			break;
		}
	}
}