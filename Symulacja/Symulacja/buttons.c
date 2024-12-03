#include "buttons.h"

void buttonInit(uint8_t pin)
{
	BUTTON_DDR &= ~(1<<pin);
	BUTTON_PORT |= (1<<pin);
}

struct button checkButton(struct button checkedButton)
{
	struct button result = {checkedButton.pin,checkedButton.state};
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
