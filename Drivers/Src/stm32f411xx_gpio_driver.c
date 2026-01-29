/**
 * @file stm32f411xx_gpio_driver.c
 * @author Andrii
 * @brief Implementation of GPIO Peripheral Driver.
 * Provides functions for GPIO initialization, reading/writing to pins/ports,
 * and handling external interrupts (EXTI).
 * @version 1.0
 * @date 2026-10-23
 */

#include "stm32f411xx_gpio_driver.h"

/******************************************************************************
*
* APIs supported by this driver
*
* For more information about the APIs check the function definitions
******************************************************************************/

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - None
 *
 * @Note              - None
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(pGPIOx == GPIOA)
			{
				GPIOA_PCLK_EN();
			}else if (pGPIOx == GPIOB)
			{
				GPIOB_PCLK_EN();
			}else if (pGPIOx == GPIOC)
			{
				GPIOC_PCLK_EN();
			}else if (pGPIOx == GPIOD)
			{
				GPIOD_PCLK_EN();
			}else if (pGPIOx == GPIOE)
			{
				GPIOE_PCLK_EN();
			}else if (pGPIOx == GPIOH)
			{
				GPIOH_PCLK_EN();
			}
		}
		else
		{
			if(pGPIOx == GPIOA)
			{
				GPIOA_PCLK_DI();
			}else if (pGPIOx == GPIOB)
			{
				GPIOB_PCLK_DI();
			}else if (pGPIOx == GPIOC)
			{
				GPIOC_PCLK_DI();
			}else if (pGPIOx == GPIOD)
			{
				GPIOD_PCLK_DI();
			}else if (pGPIOx == GPIOE)
			{
				GPIOE_PCLK_DI();
			}else if (pGPIOx == GPIOH)
			{
				GPIOH_PCLK_DI();
			}
		}

}


/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - Initializes the GPIO pin according to the specified configuration
 *
 * @param[in]         - pGPIOHandle: Pointer to the GPIO Handle structure
 *
 * @return            - None
 *
 * @Note              - Configures Mode, Speed, Pull-up/down, Output Type, and Alt Function
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	 uint32_t temp=0; //temp. register

	 //enable the peripheral clock

	 GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	//1. configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//AFLR
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7)
		{
			pGPIOHandle->pGPIOx->AFRL &= ~(0xF << ( 4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) ); //clearing
			pGPIOHandle->pGPIOx->AFRL |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << ( 4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );

		}
		else //AFHR
		{
			uint8_t pin_shift = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8;
			pGPIOHandle->pGPIOx->AFRH &= ~(0xF << ( 4 * pin_shift ) ); //clearing
			pGPIOHandle->pGPIOx->AFRH |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << ( 4 * pin_shift ) );

		}

	}

	//2. configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->OSPEEDR &= ~( 0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;

	//3. configure the pupd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->PUPDR &= ~( 0x3 << ( 2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp;


	//4. configure the optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber );
	pGPIOHandle->pGPIOx->OTYPER &= ~( 0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); //clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	//5 . configure the mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		//the non interrupt mode
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ) );
		pGPIOHandle->pGPIOx->MODER &= ~( 0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); //clearing
		pGPIOHandle->pGPIOx->MODER |= temp; //setting

	}else
	{

		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode ==GPIO_MODE_IT_FT )
		{
			//1. configure the FTSR
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode ==GPIO_MODE_IT_RT )
		{
			//1 . configure the RTSR
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->FTSR &= ~( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT )
		{
			//1. configure both FTSR and RTSR
			EXTI->RTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->FTSR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4 ;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << ( temp2 * 4);

		//3 . enable the exti interrupt delivery using IMR
		EXTI->IMR |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}

}

/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - De-initializes the GPIO port
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 *
 * @return            - None
 *
 * @Note              - Uses RCC Reset registers
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if (pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if (pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}

}


/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - Reads the value from a specific input pin
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 * @param[in]         - PinNumber: The pin number to read (0-15)
 *
 * @return            - 0 or 1
 *
 * @Note              - None
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
   uint8_t value;

   value = (uint8_t )((pGPIOx->IDR  >> PinNumber) & 0x00000001 ) ;

   return value;
}


/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             - Reads the entire input data register of the port
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 *
 * @return            - 16-bit value containing the state of all pins in the port
 *
 * @Note              - None
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;

	value = (uint16_t)pGPIOx->IDR;

	return value;
}


/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - Writes a value to a specific output pin
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 * @param[in]         - PinNumber: The pin number to write to
 * @param[in]         - Value: GPIO_PIN_SET or GPIO_PIN_RESET
 *
 * @return            - None
 *
 * @Note              - None
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{

	if(Value == GPIO_PIN_SET)
	{
		//write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx->ODR |= ( 1 << PinNumber);
	}else
	{
		//write 0
		pGPIOx->ODR &= ~( 1 << PinNumber);
	}
}


/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPort
 *
 * @brief             - Writes a value to the entire output data register
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 * @param[in]         - Value: 16-bit value to write to the port
 *
 * @return            - None
 *
 * @Note              - None
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR  = Value;
}


/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - Toggles the state of a specific output pin
 *
 * @param[in]         - pGPIOx: Base address of the gpio peripheral
 * @param[in]         - PinNumber: The pin number to toggle
 *
 * @return            - None
 *
 * @Note              - None
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR  ^= ( 1 << PinNumber);
}


/*********************************************************************
 * @fn      		  - GPIO_IRQInterruptConfig
 *
 * @brief             - Configures the NVIC for GPIO interrupts
 *
 * @param[in]         - IRQNumber: The IRQ number to configure
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - None
 *
 * @Note              - Configures ISER and ICER registers
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*NVIC_ISER0 = ( 1 << IRQNumber );

		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ISER1 register (32 to 63)
			*NVIC_ISER1 = ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ISER2 register (64 to 95)
			*NVIC_ISER2 = ( 1 << (IRQNumber % 64) );
		}
	}else
	{
		if(IRQNumber <= 31)
		{
			//program ICER0 register
			*NVIC_ICER0 = ( 1 << IRQNumber );
		}else if(IRQNumber > 31 && IRQNumber < 64 )
		{
			//program ICER1 register
			*NVIC_ICER1 = ( 1 << (IRQNumber % 32) );
		}
		else if(IRQNumber >= 64 && IRQNumber < 96 )
		{
			//program ICER2 register
			*NVIC_ICER2 = ( 1 << (IRQNumber % 64) );
		}
	}

}

/*********************************************************************
 * @fn      		  - GPIO_IRQPriorityConfig
 *
 * @brief             - Sets the priority of a GPIO interrupt
 *
 * @param[in]         - IRQNumber: The IRQ number
 * @param[in]         - IRQPriority: The priority level
 *
 * @return            - None
 *
 * @Note              - Configures the NVIC Priority Register
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section  = IRQNumber %4 ;

	uint8_t shift_amount = ( 8 * iprx_section) + ( 8 - NO_PR_BITS_IMPLEMENTED) ;

	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFF << (8 * iprx_section));
	*(  NVIC_PR_BASE_ADDR + iprx ) |=  ( IRQPriority << shift_amount );

}

/*********************************************************************
 * @fn      		  - GPIO_IRQHandling
 *
 * @brief             - Processes the GPIO interrupt
 *
 * @param[in]         - PinNumber: The pin number that triggered the interrupt
 *
 * @return            - None
 *
 * @Note              - Clears the pending bit in the EXTI Pending Register (PR)
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear the exti pr register corresponding to the pin number
	if(EXTI->PR & ( 1 << PinNumber))
	{
		//clear
		EXTI->PR |= ( 1 << PinNumber);
	}

}



