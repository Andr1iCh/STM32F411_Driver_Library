#include "stm32f411xx.h"
#include "mfrc522.h"
#include <stdio.h>

// SPI2 NSS = PB12
#define NSS_PinPB12 GPIO_PIN_NO_12
// SPI2 SCKL = PB13
#define SCKL_PinPB13 GPIO_PIN_NO_13
// SPI2 MISO = PB14
#define MISO_PinPB14 GPIO_PIN_NO_14
// SPI2 MOSI = PB15
#define MOSI_PinPB15 GPIO_PIN_NO_15
// GPIO RST = PA10
#define RST_PinPA10 GPIO_PIN_NO_10
// UART2 Tx = PA2
#define Tx_PinPA2 GPIO_PIN_NO_2

void GPIO_Initialization();
void SPI2_Initialization();
void UART2_Initialization();
void SSM_Config();

SPI_Handle_t SPI2Handler;
USART_Handle_t USART2Handler;
GPIO_Handle_t GPIOPins;
GPIO_Handle_t SPIPins;
GPIO_Handle_t CSPin;
GPIO_Handle_t RSTPin;

int main()
{
	//Hardware Initialization
	GPIO_Initialization();
	SPI2_Initialization();
	UART2_Initialization();
	SSM_Config();

	SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);

	// 2. Link Hardware to MFRC522 Driver
	MFRC522_Init(&SPI2Handler, &CSPin, &RSTPin);

	// 3. Application Logic
	MFRC522_Reset();

	Read_IT(MFRC522_REG_VERSION, 1);


	while(SPITransfrComplete == 0);

	uint8_t version;
	GetData_RxBuff(&version, 1);
	printf("Version: 0x%X\r\n", version);

	while(1);
}

void SPI2_IRQHandler ()
{
	SPI_IRQHandling(&SPI2Handler);
}

void GPIO_Initialization()
{
	//SPI2 Pins
	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = SCKL_PinPB13;
	GPIO_Init(&SPIPins);

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = MISO_PinPB14;
	GPIO_Init(&SPIPins);

	SPIPins.GPIO_PinConfig.GPIO_PinNumber = MOSI_PinPB15;
	GPIO_Init(&SPIPins);

	//SSM GPIO Pin (CS)
	CSPin.pGPIOx = GPIOB;
	CSPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	CSPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	CSPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_MEDIUM;
	CSPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	CSPin.GPIO_PinConfig.GPIO_PinNumber = NSS_PinPB12;
	GPIO_Init(&CSPin);

	//RST Pin
	RSTPin.pGPIOx = GPIOA;
	RSTPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	RSTPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	RSTPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	RSTPin.GPIO_PinConfig.GPIO_PinNumber = RST_PinPA10;
	GPIO_Init(&RSTPin);
}

void SPI2_Initialization()
{
	SPI2Handler.pSPIx = SPI2;
	SPI2Handler.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handler.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handler.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handler.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handler.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handler.SPIConfig.SPI_SSM = SPI_SSM_DI;
	SPI2Handler.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;

	SPI_Init(&SPI2Handler);
}

void UART2_Initialization(void)
{
    GPIOPins.pGPIOx= GPIOA;
    GPIOPins.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    GPIOPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    GPIOPins.GPIO_PinConfig.GPIO_PinNumber = Tx_PinPA2;
    GPIOPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_Init(&GPIOPins);

    USART2Handler.pUSARTx = USART2;
    USART2_PCLK_EN();

    // Baud Rate HSI = 16MHz clock for 115200 baud
    USART2Handler.pUSARTx->USART_BRR = 0x008B;

    USART2Handler.pUSARTx->USART_CR1 |=(1<<USART_CR1_TE);
    USART2Handler.pUSARTx->USART_CR1 |=(1<<USART_CR1_UE);
}

void SSM_Config()
{
	SPI2Handler.pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSM);
	SPI2Handler.pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSI);
}

int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++)
    {
        while (!(USART2Handler.pUSARTx->USART_SR & (1<<USART_SR_TXE)));
        USART2Handler.pUSARTx->USART_DR = ptr[i];
    }
    return len;
}
