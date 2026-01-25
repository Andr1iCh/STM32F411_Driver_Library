/*
 * 02_led_button.c
 *
 *  Created on: Oct 28, 2025
 *      Author: Andy
 */

#include "stm32f411xx.h"

void delay()
{
	for (uint32_t i = 0; i<500000/2; i++ );
}
int main (void)
{
	GPIO_Handle_t GpioLed;
		GpioLed.pGPIOx = GPIOA;
		GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
		GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
		GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
		GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GpioLed);
	GPIO_Handle_t GpioButton;
		GpioButton.pGPIOx = GPIOC;
		GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
		GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
		GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
		GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GPIO_Init(&GpioButton);

	while(1)
	{
		if(!GPIO_ReadFromInputPin(GpioButton.pGPIOx, GPIO_PIN_NO_13))
				{
					delay();
					GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);

				}

	}


}

