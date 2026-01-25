#include "stm32f411xx.h"
#include <string.h>
/*
 * SPI2:
 * PB10 = CLK
 * PB12 = NSS
 * PB14	= MISO
 * PB15 = MOSI
 * alternate mode 5
 */
void SPI2_GPIOInits()
{
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	/*//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
	*/
}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_CPHA =	SPI_CPHA_HIGH;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_EN;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;

	SPI_Init(&SPI2handle);
}

int main()
{
	char user_data[]="Hello world";

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	// enable the SSI bit to HIGH
	SPI_SSIConfig(SPI2, ENABLE);

	// enable SPI2 peripheral
	SPI_PeripheralControl(SPI2, ENABLE);

	//send length of information
	uint8_t dataLen = strlen(user_data);
	SPI_SendData(SPI2,&dataLen, 1);

	//send data
	SPI_SendData(SPI2,(uint8_t*)user_data, strlen(user_data));

	//confirm SPI is not busy
	while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	// disable SPI2 peripheral
	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
