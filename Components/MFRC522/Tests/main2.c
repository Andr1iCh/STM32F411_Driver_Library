#include "stm32f411xx.h"
#include <stdio.h>
#include <string.h>

// --- Pin Definitions ---
#define NSS_PinPB12 GPIO_PIN_NO_12
#define SCKL_PinPB13 GPIO_PIN_NO_13
#define MISO_PinPB14 GPIO_PIN_NO_14
#define MOSI_PinPB15 GPIO_PIN_NO_15
#define RST_PinPA10 GPIO_PIN_NO_10
#define Tx_PinPA2 GPIO_PIN_NO_2

// --- MFRC522 Registers ---
#define MFRC522_REG_COMMAND       0x01
#define MFRC522_REG_COMIEN        0x02
#define MFRC522_REG_DIVLEN        0x03
#define MFRC522_REG_COMIRQ        0x04
#define MFRC522_REG_DIVIRQ        0x05
#define MFRC522_REG_ERROR         0x06
#define MFRC522_REG_STATUS1       0x07
#define MFRC522_REG_STATUS2       0x08
#define MFRC522_REG_FIFO_DATA     0x09
#define MFRC522_REG_FIFO_LEVEL    0x0A
#define MFRC522_REG_CONTROL       0x0C
#define MFRC522_REG_BIT_FRAMING   0x0D
#define MFRC522_REG_COLL          0x0E
#define MFRC522_REG_MODE          0x11
#define MFRC522_REG_TX_CONTROL    0x14
#define MFRC522_REG_TX_ASK        0x15
#define MFRC522_REG_VERSION       0x37

// --- MFRC522 Commands ---
#define PCD_IDLE                  0x00
#define PCD_CALCCRC               0x03
#define PCD_TRANSMIT              0x04
#define PCD_RECEIVE               0x08
#define PCD_TRANSCEIVE            0x0C
#define PCD_AUTHENT               0x0E
#define PCD_RESETPHASE            0x0F

// --- MIFARE Commands ---
#define PICC_REQIDL               0x26
#define PICC_ANTICOLL             0x93

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

#define MAX_LEN_BYTES 64
#define MFRC522_READ_BIT    (1 << 7)
#define MFRC522_ADDR_MASK   (0x3F)   // 0011 1111 mask for safety

void GPIO_Initialization();
void SPI2_Initialization();
void UART2_Initialization();
void MFRC522_Initialization();
void SSM_Config();
void SMM_NSS(uint8_t SetOrRst);

void Read_IT(uint8_t addrByte, uint8_t lenByte);
uint8_t GetData_RxBuff(uint8_t *destination, uint8_t length);
void Write_IT(uint8_t addrByte, uint8_t *pBuffer, uint8_t lenByte);
void MFRC522_Reset ();


SPI_Handle_t SPI2Handler;
USART_Handle_t USART2Handler;
GPIO_Handle_t SPIPins;
GPIO_Handle_t GPIOPins;

uint8_t RxBuff[MAX_LEN_BYTES];
uint8_t TxBuff[MAX_LEN_BYTES];

//flags
volatile uint8_t SPITransfrComplete;


int main()
{

	SPITransfrComplete = 1;
	GPIO_Initialization();
	SPI2_Initialization();
	UART2_Initialization();

	SSM_Config();

	SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);
	printf("System Initialized.\r\n");

	//MFRC522 Reset and Init
	MFRC522_Reset ();
	MFRC522_Initialization();

	uint8_t versionReg = 0x37;
	Read_IT(versionReg, 1);
	while(SPITransfrComplete == 0);
	uint8_t version;
	GetData_RxBuff(&version, 1);
	printf("version: %x\r\n", version);



	while(1);


}


void MFRC522_Initialization()
{
	uint8_t tBuff = 0x3D;
	Write_IT(MFRC522_REG_MODE, tBuff, 1);
	while(SPITransfrComplete == 0);
	tBuff=0x40;
	Write_IT(MFRC522_REG_TX_ASK, tBuff, 1);
	while(SPITransfrComplete == 0);

	MFRC522_AntennaOn();
	MFRC522_AntennaOn();
}

void MFRC522_AntennaOn() {
	uint8_t temp;
	Read_IT(MFRC522_REG_TX_CONTROL, 1);
	GetData_RxBuff(temp, 1);
	if (!(temp & 0x03)) {
		MFRC522_WriteReg(MFRC522_REG_TX_CONTROL, temp | 0x03);
	}
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

	SPI_PeripheralControl(SPI2, ENABLE);
	SMM_NSS(RESET);


	SPI_ReceiveDataInterruptMode(&SPI2Handler, RxBuff, totalLen);
	SPI_SendDataInterruptMode(&SPI2Handler, TxBuff, totalLen);

}

void Write_IT(uint8_t addrByte, uint8_t *pBuffer, uint8_t lenByte)
{
	if((lenByte>512)||(lenByte<1))
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

	SPI_PeripheralControl(SPI2, ENABLE);
	SMM_NSS(RESET);


	SPI_ReceiveDataInterruptMode(&SPI2Handler, RxBuff, totalLen);
	SPI_SendDataInterruptMode(&SPI2Handler, TxBuff, totalLen);

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
	 * 16000000*0.05 = 800000 /4 = 200000 => 50ms delay
	*/
	volatile uint32_t count =200000;
	GPIO_WriteToOutputPin(GPIOPins.pGPIOx, RST_PinPA10, RESET);
	while(count--);
	GPIO_WriteToOutputPin(GPIOPins.pGPIOx, RST_PinPA10, SET);

	//stabilization delay
	count =50000;
	while(count--);

	//SOFT RESET
	uint8_t CommandRegAddr = 0x01;
	uint8_t SoftResetCmd = 0xF;

	Write_IT(CommandRegAddr, &SoftResetCmd, 1);
	while(SPITransfrComplete == 0);
}

void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent)
{

	if(AppEvent==SPI_EVENT_RX_CMPLT)
	{
		SMM_NSS(SET);
		SPI_PeripheralControl(SPI2, DISABLE);
		SPITransfrComplete = 1;
	}

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

	//SSM GPIO Pin
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = NSS_PinPB12;
	GPIO_Init(&SPIPins);

	//RST Pin
	GPIOPins.pGPIOx = GPIOA;
	GPIOPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GPIOPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GPIOPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOPins.GPIO_PinConfig.GPIO_PinNumber = RST_PinPA10;
	GPIO_Init(&GPIOPins);
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
    //Configure PA2 as TX
    GPIOPins.pGPIOx= GPIOA;
    GPIOPins.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    GPIOPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    GPIOPins.GPIO_PinConfig.GPIO_PinNumber = Tx_PinPA2;
    GPIOPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_Init(&GPIOPins);

    USART2Handler.pUSARTx = USART2;

    //Enable Clock for UART2
    USART2_PCLK_EN();

    // Baud Rate HSI = 16MHz clock for 115200 baud
    USART2Handler.pUSARTx->USART_BRR = 0x008B;

    //Enable transmitter and UART
    USART2Handler.pUSARTx->USART_CR1 |=(1<<USART_CR1_TE);
    USART2Handler.pUSARTx->USART_CR1 |=(1<<USART_CR1_UE);

}

void SSM_Config()
{
	SPI2Handler.pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSM);
	SPI2Handler.pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSI);

}

void SMM_NSS(uint8_t SetOrRst)
{
	GPIO_WriteToOutputPin(SPIPins.pGPIOx, NSS_PinPB12, SetOrRst);
}

int _write(int file, char *ptr, int len) {
    for (int i = 0; i < len; i++)
    {
        while (!(USART2Handler.pUSARTx->USART_SR & (1<<USART_SR_TXE)));
        USART2Handler.pUSARTx->USART_DR = ptr[i];
    }
    return len;
}
