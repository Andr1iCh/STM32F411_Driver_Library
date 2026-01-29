/**
 * @file stm32f411xx_usart_driver.h
 * @author Andrii
 * @brief Header file for USART Peripheral Driver.
 * Contains configuration structures and API prototypes for UART/USART communication.
 * @version 1.0
 * @date 2026-01-27
 */

/**
 * @defgroup USART_Driver USART Driver
 * @brief USART Peripheral Driver
 * @ingroup STM32F411xx_Drivers
 * @{
 */
#ifndef INC_STM32F411XX_USART_DRIVER_H_
#define INC_STM32F411XX_USART_DRIVER_H_

#include "stm32f411xx.h"

/**
 * @brief  Handle structure for USART/UART peripheral control.
 * @note   Holds the base address of the peripheral instance.
 */
typedef struct
{
	USART_RegDef_t *pUSARTx; 			/*!< Base address of the USARTx peripheral */

}USART_Handle_t;

#endif /* INC_STM32F411XX_USART_DRIVER_H_ */
/** @} */
