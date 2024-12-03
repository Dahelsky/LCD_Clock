#include "button.h"

void buttonInit(uint8_t pin)
{
	BUTTON_DDR &= ~(1<<pin); //input 
	BUTTON_PORT |= (1<<pin); //pullup
}

button checkButton(button checkedButton)
{
	struct button result = checkedButton;
	if(!(BUTTON_PIN & (1<<checkedButton.pin)))
	{
		switch(checkedButton.state)
		{
			case 0:
			result.state = 1;
			return result;
			case 1:
			result.state = 2;
			return result;
		}
	}
	else
	{
		switch(checkedButton.state)
		{
			case 3:
			result.state =  4;
			return result;
			case 4:
			result.state =  0;
			return result;
		}
	}
	return result;
}

uint8_t isButtonClicked(button checkedButton)
{
	if(checkedButton.state == 2)
		return 0;
	return 1;
}


void confirmClick(button* Button)
{
	Button->state = 3;
}
