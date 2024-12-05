#include "button.h"


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
		return 1;
	return 0;
}


void confirmClick(button* Button)
{
	Button->state = 3;
}
