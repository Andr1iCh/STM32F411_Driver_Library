/**
 * @file stm32f411xx_spi_driver.h
 * @author Andrii
 * @brief Header file for SPI Peripheral Driver.
 * Defines configuration structures for SPI communication (Master/Slave),
 * application states, events, and API prototypes.
 * @version 1.0
 * @date 2026-12-12
 */

/**
 * @defgroup SPI_Driver SPI Driver
 * @brief SPI Peripheral Driver Configuration and APIs
 * @ingroup STM32F411xx_Drivers
 * @{
 */
#ifndef INC_STM32F411XX_SPI_DRIVER_H_
#define INC_STM32F411XX_SPI_DRIVER_H_

#include "stm32f411xx.h"

/******************************************************************************************
 * Data Structures
 ******************************************************************************************/

/**
 * @brief  Configuration structure for SPIx peripheral.
 * @note   This structure is used to configure the communications parameters
 * (Mode, Bus Config, Clock Speed, DFF, CPHA, CPOL, SSM).
 */
typedef struct
{
	uint8_t SPI_DeviceMode;		/*!< Master/Slave mode selection. Possible values: @ref SPI_DeviceMode */
	uint8_t SPI_BusConfig;		/*!< Full-duplex/Half-duplex/Simplex config. Possible values: @ref SPI_BusConfig */
	uint8_t SPI_SclkSpeed;		/*!< Baud rate control (Prescaler). Possible values: @ref SPI_SclkSpeed */
	uint8_t SPI_DFF;			/*!< Data Frame Format (8/16 bits). Possible values: @ref SPI_DFF */
	uint8_t SPI_CPHA;			/*!< Clock Phase. Possible values: @ref SPI_CPHA */
	uint8_t SPI_CPOL;			/*!< Clock Polarity. Possible values: @ref SPI_CPOL */
	uint8_t SPI_SSM;			/*!< Software Slave Management. Possible values: @ref SPI_SSM */
}SPI_Config_t;

/**
 * @brief  Handle structure for SPI peripheral.
 * @note   Holds the base address of the SPI peripheral instance and its configuration settings.
 */
typedef struct
{
	SPI_RegDef_t *pSPIx;        	/*!< This holds the base address of the SPIx peripheral >*/
	SPI_Config_t SPIConfig;			/*!< This holds SPI configuration settings >*/
	uint8_t      *pTxBuffer;		/*!< To store the app. Tx buffer address >*/
	uint8_t      *pRxBuffer;    	/*!< To store the app. Rx buffer address >*/
	uint32_t     TxLen;         	/*!< To store Tx len >*/
	uint32_t     RxLen;         	/*!< To store Rx len >*/
	uint32_t     TxState;       	/*!< To store Tx state >*/
	uint32_t     RxState;       	/*!< To store Rx state >*/
}SPI_Handle_t;

/******************************************************************************************
 * Macros & Constants
 ******************************************************************************************/

/** @defgroup SPI_App_States SPI Application States
  * @{
  */
#define SPI_READY 					0
#define SPI_BUSY_IN_RX 				1
#define SPI_BUSY_IN_TX 				2
/** @} */

/** @defgroup SPI_Events SPI Application Events
  * @{
  */
#define SPI_EVENT_TX_CMPLT   1
#define SPI_EVENT_RX_CMPLT   2
#define SPI_EVENT_OVR_ERR    3
#define SPI_EVENT_CRC_ERR    4
/** @} */

/** @defgroup SPI_DeviceMode SPI Device Modes
  * @{
  */
#define SPI_DEVICE_MODE_MASTER    1
#define SPI_DEVICE_MODE_SLAVE     0
/** @} */

/** @defgroup SPI_BusConfig SPI Bus Configurations
  * @{
  */
#define SPI_BUS_CONFIG_FD                1
#define SPI_BUS_CONFIG_HD                2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    3
/** @} */

/** @defgroup SPI_SclkSpeed SPI Clock Speed Prescalers
  * @{
  */
#define SPI_SCLK_SPEED_DIV2             	0
#define SPI_SCLK_SPEED_DIV4             	1
#define SPI_SCLK_SPEED_DIV8             	2
#define SPI_SCLK_SPEED_DIV16             	3
#define SPI_SCLK_SPEED_DIV32             	4
#define SPI_SCLK_SPEED_DIV64             	5
#define SPI_SCLK_SPEED_DIV128             	6
#define SPI_SCLK_SPEED_DIV256             	7
/** @} */

/** @defgroup SPI_DFF SPI Data Frame Formats
  * @{
  */
#define SPI_DFF_8BITS 	0
#define SPI_DFF_16BITS  1
/** @} */

/** @defgroup SPI_CPOL SPI Clock Polarity
  * @{
  */
#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW 0
/** @} */

/** @defgroup SPI_CPHA SPI Clock Phase
  * @{
  */
#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW 0
/** @} */

/** @defgroup SPI_SSM SPI Software Slave Management
  * @{
  */
#define SPI_SSM_EN     1
#define SPI_SSM_DI     0


/** @defgroup SPI_Flags SPI Status Flags
  * @{
  */
#define SPI_TXE_FLAG    ( 1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG   ( 1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG   ( 1 << SPI_SR_BSY)
/** @} */

/******************************************************************************************
 * APIs supported by this driver
 * For more information about the APIs check the function definitions
 ******************************************************************************************/

/** @defgroup SPI_APIs SPI APIs
  * @{
  */

/* Peripheral Clock setup */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/* Init and De-init */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_Handle_t *pSPIHandle);

/* Data Send and Receive */
void SPI_SendData(SPI_RegDef_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI_SendDataInterruptMode(SPI_Handle_t *pSPIx,uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataInterruptMode(SPI_Handle_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/* IRQ Configuration and ISR handling */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/* Other Peripheral Control APIs */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/* Application callback */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent);
/** @} */

/** @} */
#endif /* INC_STM32F411XX_SPI_DRIVER_H_ */
