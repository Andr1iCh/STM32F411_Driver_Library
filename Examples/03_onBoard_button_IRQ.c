#include "stm32f411xx.h"

int main(void)
{
	//PC13
	//rt

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
			GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_RT;
			GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
			GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
		GPIO_Init(&GpioButton);

	//Exception
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, 15);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);



}

void EXTI15_10_IRQHandler  (void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_13);
	GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
