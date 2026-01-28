/*
 * stm32f411xx_usart_driver.h
 *
 *  Created on: Jan 27, 2026
 *      Author: Andre
 */

#ifndef INC_STM32F411XX_USART_DRIVER_H_
#define INC_STM32F411XX_USART_DRIVER_H_

#include "stm32f411xx.h"

typedef struct
{
	USART_RegDef_t *pUSARTx; 			/* This holds the base address of the USARTx peripheral */

}USART_Handle_t;

#endif /* INC_STM32F411XX_USART_DRIVER_H_ */
