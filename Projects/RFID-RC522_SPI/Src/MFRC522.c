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

// Initialization Function to link hardware with driver
void MFRC522_Init(SPI_Handle_t *pSPI, GPIO_Handle_t *pCS, GPIO_Handle_t *pRST)
{
	pSPI_Handle = pSPI;
	pCS_Pin = pCS;
	pRST_Pin = pRST;
}

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

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent)
{
	if(AppEvent==SPI_EVENT_RX_CMPLT)
	{
		SSM_NSS(SET);
		SPI_PeripheralControl(pSPI_Handle->pSPIx, DISABLE);
		SPITransfrComplete = 1;
	}
}
