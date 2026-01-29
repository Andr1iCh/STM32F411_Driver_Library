/**
 * @file mfrc522.c
 * @author Andrii
 * @brief Implementation of MFRC522 RFID module driver.
 *
 * Handles initialization, low-level register read/write operations
 * using the STM32 SPI driver, and hardware reset control.
 *
 * @version 1.0
 * @date 2026-01-28
 */

#include "mfrc522.h"
#include <string.h>
#include <stdio.h>

// Global buffers
static uint8_t RxBuff[MAX_LEN_BYTES];
static uint8_t TxBuff[MAX_LEN_BYTES];

// Private pointers to hardware Handles
static SPI_Handle_t *pSPI_Handle;
static GPIO_Handle_t *pCS_Pin;
static GPIO_Handle_t *pRST_Pin;

// Global flag definition
volatile uint8_t SPITransfrComplete = 1;

// Internal Helper function
static void SSM_NSS(uint8_t SetOrRst)
{
	GPIO_WriteToOutputPin(pCS_Pin->pGPIOx, pCS_Pin->GPIO_PinConfig.GPIO_PinNumber, SetOrRst);
}

/*********************************************************************
 * @fn      		  - MFRC522_Init
 *
 * @brief             - Initializes the driver by linking hardware handles
 *
 * @param[in]         - pSPI: Pointer to the SPI Handle
 * @param[in]         - pCS:  Pointer to the GPIO Handle for Chip Select
 * @param[in]         - pRST: Pointer to the GPIO Handle for Reset pin
 *
 * @return            - None
 *
 * @Note              - Does not start communication, only initializes pointers
 */
void MFRC522_Init(SPI_Handle_t *pSPI, GPIO_Handle_t *pCS, GPIO_Handle_t *pRST)
{
	pSPI_Handle = pSPI;
	pCS_Pin = pCS;
	pRST_Pin = pRST;
}

/*********************************************************************
 * @fn      		  - Read_IT
 *
 * @brief             - Reads data from MFRC522 register using Interrupt-based SPI
 *
 * @param[in]         - addrByte: Register address to read from
 * @param[in]         - lenByte:  Number of bytes to read
 *
 * @return            - None
 *
 * @Note              - Non-blocking call. Check SPITransfrComplete flag before reading data.
 */
void Read_IT(uint8_t addrByte, uint8_t lenByte)
{
	if((lenByte>MAX_LEN_BYTES-1)||(lenByte<1))
	{
		printf("Incorrect length error\r\n");
		return;
	}

	if(!SPITransfrComplete)
	{
		return;
	}

	SPITransfrComplete = 0;

	//packing for transmission
	addrByte = ((addrByte & MFRC522_ADDR_MASK)<<1)|MFRC522_READ_BIT;
	TxBuff[0] = addrByte;
	memset(&TxBuff[1], 0x00, lenByte);
	uint8_t totalLen = lenByte + 1;

	SPI_PeripheralControl(pSPI_Handle->pSPIx, ENABLE);
	SSM_NSS(RESET);

	SPI_ReceiveDataInterruptMode(pSPI_Handle, RxBuff, totalLen);
	SPI_SendDataInterruptMode(pSPI_Handle, TxBuff, totalLen);
}

/*********************************************************************
 * @fn      		  - WriteBuffer_IT
 *
 * @brief             - Writes a buffer of data to MFRC522 register
 *
 * @param[in]         - addrByte: Target register address
 * @param[in]         - pBuffer:  Pointer to data buffer
 * @param[in]         - lenByte:  Length of data
 *
 * @return            - None
 *
 * @Note              - Non-blocking call. Used for sending commands or data packets.
 */
void WriteBuffer_IT(uint8_t addrByte, const uint8_t *pBuffer, uint8_t lenByte)
{
	if((lenByte>MAX_LEN_BYTES-1)||(lenByte<1))
	{
		printf("Incorrect length error\n");
		return;
	}
	if(!SPITransfrComplete)
	{
		return;
	}

	SPITransfrComplete = 0;

	//packing for transmission
	addrByte = ((addrByte & MFRC522_ADDR_MASK)<<1);
	TxBuff[0] = addrByte;
	memcpy(&TxBuff[1], pBuffer, lenByte);
	uint8_t totalLen = lenByte + 1;

	SPI_PeripheralControl(pSPI_Handle->pSPIx, ENABLE);
	SSM_NSS(RESET);

	SPI_ReceiveDataInterruptMode(pSPI_Handle, RxBuff, totalLen);
	SPI_SendDataInterruptMode(pSPI_Handle, TxBuff, totalLen);
}

/*********************************************************************
 * @fn      		  - WriteByte_IT
 *
 * @brief             - Writes a single byte to a specific register
 *
 * @param[in]         - addrByte: Target register address
 * @param[in]         - value:    Byte value to write
 *
 * @return            - None
 *
 * @Note              - Helper wrapper for single-byte transactions
 */
void WriteByte_IT(uint8_t addrByte, uint8_t value)
{
	if(!SPITransfrComplete)
	{
		return;
	}

	SPITransfrComplete = 0;

	//packing for transmission
	addrByte = ((addrByte & MFRC522_ADDR_MASK)<<1);
	TxBuff[0] = addrByte;
	TxBuff[1] = value;

	uint8_t totalLen = 2;

	SPI_PeripheralControl(pSPI_Handle->pSPIx, ENABLE);
	SSM_NSS(RESET);

	SPI_ReceiveDataInterruptMode(pSPI_Handle, RxBuff, totalLen);
	SPI_SendDataInterruptMode(pSPI_Handle, TxBuff, totalLen);
}

/*********************************************************************
 * @fn      		  - GetData_RxBuff
 *
 * @brief             - Copies received data from internal driver buffer to user buffer
 *
 * @param[in]         - destination: Pointer to user application buffer
 * @param[in]         - length:      Number of bytes to copy
 *
 * @return            - 1 if successful, 0 if SPI transfer is still busy
 *
 * @Note              - Should be called after SPITransfrComplete is set
 */
uint8_t GetData_RxBuff(uint8_t *destination, uint8_t length)
{
	if (!SPITransfrComplete)
	{
		return 0;
	}

	if (length > (MAX_LEN_BYTES - 1))
	{
		length = MAX_LEN_BYTES - 1;
	}

	memcpy(destination, &RxBuff[1], length);
	return 1;
}

/*********************************************************************
 * @fn      		  - MFRC522_Reset
 *
 * @brief             - Performs hardware (pin) and software reset of the module
 *
 * @param[in]         - None
 *
 * @return            - None
 *
 * @Note              - Contains blocking delays for oscillator stabilization
 */
void MFRC522_Reset ()
{
	//TODO Do normal timer implementation
	//HARD RESET
	/*soft defined delay for 16MHz
	 * 16000000*0.05 = 800000 /4
	*/
	volatile uint32_t count =200000;

	// Using the stored RST pin pointer
	GPIO_WriteToOutputPin(pRST_Pin->pGPIOx, pRST_Pin->GPIO_PinConfig.GPIO_PinNumber, RESET);
	while(count--);
	GPIO_WriteToOutputPin(pRST_Pin->pGPIOx, pRST_Pin->GPIO_PinConfig.GPIO_PinNumber, SET);

	//stabilization delay
	count =50000;
	while(count--);

	//SOFT RESET
	WriteByte_IT(MFRC522_REG_COMMAND, MFRC522_CMD_SOFT_RESET);
	while(SPITransfrComplete == 0);
}

/*********************************************************************
 * @fn      		  - SPI_ApplicationEventCallback
 *
 * @brief             - Handles SPI events to manage Chip Select line
 *
 * @param[in]         - pSPIHandle: Pointer to SPI Handle
 * @param[in]         - AppEvent:   Event code (e.g. SPI_EVENT_RX_CMPLT)
 *
 * @return            - None
 *
 * @Note              - Called automatically by SPI ISR
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent)
{
	if(AppEvent==SPI_EVENT_RX_CMPLT)
	{
		SSM_NSS(SET);
		SPI_PeripheralControl(pSPI_Handle->pSPIx, DISABLE);
		SPITransfrComplete = 1;
	}
}
