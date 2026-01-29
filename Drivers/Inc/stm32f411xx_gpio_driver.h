/**
 * @file stm32f411xx_gpio_driver.h
 * @author Andrii
 * @brief Header file for GPIO Peripheral Driver.
 * Defines configuration structures, macros for pin modes, speeds,
 * output types, and API prototypes for GPIO initialization and control.
 * @version 1.0
 * @date 2026-10-23
 */

/**
 * @defgroup GPIO_Driver GPIO Driver
 * @brief GPIO Peripheral Driver
 * @ingroup STM32F411xx_Drivers
 * @{
 */
#ifndef INC_STM32F411XX_GPIO_DRIVER_H_
#define INC_STM32F411XX_GPIO_DRIVER_H_

#include "stm32f411xx.h"

/******************************************************************************************
 * Data Structures
 ******************************************************************************************/

/**
 * @brief  Configuration structure for a GPIO pin.
 * @note   This structure configures the specific pin parameters
 * (Pin Number, Mode, Speed, Output Type, Pull-up/Pull-down).
 */
typedef struct
{
	uint8_t GPIO_PinNumber;        /*!< Possible values from @ref GPIO_PIN_NUMBERS >*/
	uint8_t GPIO_PinMode;          /*!< Possible values from @ref GPIO_PIN_MODES >*/
	uint8_t GPIO_PinSpeed;         /*!< Possible values from @ref GPIO_PIN_SPEED >*/
	uint8_t GPIO_PinPuPdControl;   /*!< Possible values from @ref GPIO_PIN_PUPD >*/
	uint8_t GPIO_PinOPType;        /*!< Possible values from @ref GPIO_PIN_OUTPUT_TYPES >*/
	uint8_t GPIO_PinAltFunMode;    /*!< Possible values from Alternate function modes>*/
}GPIO_PinConfig_t;

/**
 * @brief  Handle structure for a GPIO pin.
 * @note   Holds the base address of the GPIO port to which the pin belongs
 * and its configuration settings.
 */
typedef struct
{
	GPIO_RegDef_t *pGPIOx;          /*!< This holds the base address of the GPIO port to which the pin belongs >*/
	GPIO_PinConfig_t GPIO_PinConfig;/*!< This holds GPIO pin configuration settings >*/
}GPIO_Handle_t;

/******************************************************************************************
 * Macros & Constants
 ******************************************************************************************/

/** @defgroup GPIO_PIN_NUMBERS GPIO Pin Numbers
  * @{
  */
#define GPIO_PIN_NO_0  				0
#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2  				2
#define GPIO_PIN_NO_3  				3
#define GPIO_PIN_NO_4  				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_8  				8
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10  			10
#define GPIO_PIN_NO_11 				11
#define GPIO_PIN_NO_12  			12
#define GPIO_PIN_NO_13 				13
#define GPIO_PIN_NO_14 				14
#define GPIO_PIN_NO_15 				15
/** @} */

/** @defgroup GPIO_PIN_MODES GPIO Pin Possible Modes
  * @{
  */
#define GPIO_MODE_IN 		0  /*!< Input mode */
#define GPIO_MODE_OUT 		1  /*!< Output mode */
#define GPIO_MODE_ALTFN 	2  /*!< Alternate function mode */
#define GPIO_MODE_ANALOG 	3  /*!< Analog mode */
#define GPIO_MODE_IT_FT     4  /*!< Interrupt Falling Edge Trigger */
#define GPIO_MODE_IT_RT     5  /*!< Interrupt Rising Edge Trigger */
#define GPIO_MODE_IT_RFT    6  /*!< Interrupt Rising & Falling Edge Trigger */
/** @} */

/** @defgroup GPIO_PIN_OUTPUT_TYPES GPIO Pin Output Types
  * @{
  */
#define GPIO_OP_TYPE_PP   0  /*!< Output Push-Pull */
#define GPIO_OP_TYPE_OD   1  /*!< Output Open-Drain */
/** @} */

/** @defgroup GPIO_PIN_SPEED GPIO Pin Output Speeds
  * @{
  */
#define GPIO_SPEED_LOW			0
#define GPIO_SPEED_MEDIUM		1
#define GPIO_SPEED_FAST			2
#define GPIO_SPEED_HIGH			3
/** @} */

/** @defgroup GPIO_PIN_PUPD GPIO Pin Pull-Up/Pull-Down Config
  * @{
  */
#define GPIO_NO_PUPD   		0 /*!< No Pull-up or Pull-down */
#define GPIO_PIN_PU			1 /*!< Pull-up */
#define GPIO_PIN_PD			2 /*!< Pull-down */
/** @} */

/******************************************************************************************
 * APIs supported by this driver
 * For more information about the APIs check the function definitions
 ******************************************************************************************/

/** @defgroup GPIO_APIs GPIO APIs
  * @{
  */
/* Peripheral Clock setup */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/* Init and De-init */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/* Data read and write */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/* IRQ Configuration and ISR handling */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);
/** @} */

#endif /* INC_STM32F411XX_GPIO_DRIVER_H_ */
/** @} */
