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

// --- Constants ---
#define MAX_LEN_BYTES 64
#define MFRC522_READ_BIT    (1 << 7)

// --- Function Prototypes ---
void GPIO_Initialization();
void SPI2_Initialization();
void UART2_Initialization();
void SSM_Config();
void SMM_NSS(uint8_t SetOrRst);

// MFRC522 Core
uint8_t SPI_TransmitReceiveByte(uint8_t data); // CORE FIX
void MFRC522_WriteReg(uint8_t addr, uint8_t val);
uint8_t MFRC522_ReadReg(uint8_t addr);
void MFRC522_Init();
void MFRC522_HardReset();
void MFRC522_AntennaOn();
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen);
uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *tagType);
uint8_t MFRC522_Anticoll(uint8_t *serNum);

// --- Global Handles ---
SPI_Handle_t SPI2Handler;
USART_Handle_t USART2Handler;
GPIO_Handle_t SPIPins;
GPIO_Handle_t GPIOPins;

int main()
{
	GPIO_Initialization();
	SPI2_Initialization();
	UART2_Initialization();
	SSM_Config();

    // Вмикаємо SPI один раз і назавжди для блокуючого режиму
	SPI_PeripheralControl(SPI2, ENABLE);

	printf("System Initialized.\r\n");

	// 1. Hardware Reset & Init
	MFRC522_HardReset();
	MFRC522_Init();

	uint8_t version = MFRC522_ReadReg(MFRC522_REG_VERSION);
	printf("MFRC522 Version: 0x%X (Expected: 0x91 or 0x92)\r\n", version);

	uint8_t CardID[5];
	uint8_t str[MAX_LEN_BYTES];

	while(1)
	{
		// 2. Request (Find card)
		// Використовуємо REQUEST IDLE, щоб знайти нові картки
		if (MFRC522_Request(PICC_REQIDL, str) == 1)
		{
			// 3. Anticollision (Read UID)
			if (MFRC522_Anticoll(CardID) == 1)
			{
				printf("UID: %02X-%02X-%02X-%02X\r\n", CardID[0], CardID[1], CardID[2], CardID[3]);

                // Зупинка, щоб не спамити UID однієї і тієї ж картки
                // Можна додати команду Halt, але поки просто затримка
                for(volatile int j=0; j<1000000; j++);
			}
		}
        // Невелика затримка між скануваннями
        for(volatile int i=0; i<100000; i++);
	}
}

// --- MFRC522 Implementation ---

// CRITICAL FIX: Блокуючий обмін байт-за-байт
// Це запобігає Overrun Error, який трапляється, якщо відправити 2 байти підряд без читання.
uint8_t SPI_TransmitReceiveByte(uint8_t data) {
    // 1. Чекаємо, поки буфер передачі стане порожнім (TXE)
    while( ! (SPI2->SPI_SR & (1 << SPI_SR_TXE)) );

    // 2. Відправляємо дані
    SPI2->SPI_DR = data;

    // 3. Чекаємо, поки прийдуть дані (RXNE)
    // SPI завжди отримує байт у відповідь на відправлений байт
    while( ! (SPI2->SPI_SR & (1 << SPI_SR_RXNE)) );

    // 4. Читаємо отримане (і чистимо прапорець RXNE)
    return (uint8_t)(SPI2->SPI_DR);
}

void MFRC522_WriteReg(uint8_t addr, uint8_t val) {
    SMM_NSS(RESET); // CS Low

    // Адреса для запису: (Addr << 1) & 0x7E
    SPI_TransmitReceiveByte((addr << 1) & 0x7E);
    // Дані
    SPI_TransmitReceiveByte(val);

    SMM_NSS(SET);   // CS High
}

uint8_t MFRC522_ReadReg(uint8_t addr) {
    uint8_t val;
    SMM_NSS(RESET); // CS Low

    // Адреса для читання: ((Addr << 1) & 0x7E) | 0x80
    SPI_TransmitReceiveByte(((addr << 1) & 0x7E) | 0x80);
    // Відправляємо пустий байт (Dummy), щоб виштовхати дані з MFRC522
    val = SPI_TransmitReceiveByte(0x00);

    SMM_NSS(SET);   // CS High
    return val;
}

void MFRC522_Init() {
	MFRC522_WriteReg(MFRC522_REG_COMMAND, PCD_RESETPHASE);
	for(volatile int i=0; i<50000; i++); // Wait for reset

	MFRC522_WriteReg(MFRC522_REG_MODE, 0x3D);
    MFRC522_WriteReg(MFRC522_REG_TX_ASK, 0x40);
	MFRC522_AntennaOn();
}

void MFRC522_AntennaOn() {
	uint8_t temp = MFRC522_ReadReg(MFRC522_REG_TX_CONTROL);
	if (!(temp & 0x03)) {
		MFRC522_WriteReg(MFRC522_REG_TX_CONTROL, temp | 0x03);
	}
}

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, uint16_t *backLen) {
	uint8_t status = 0; // Error
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t n;
	uint16_t i;

	if (command == PCD_AUTHENT) {
		irqEn = 0x12;
		waitIRq = 0x10;
	} else if (command == PCD_TRANSCEIVE) {
		irqEn = 0x77;
		waitIRq = 0x30;
	}

	MFRC522_WriteReg(MFRC522_REG_COMMAND, PCD_IDLE);

    // Очистити FIFO і прапорці переривань
    uint8_t fifoLevel = MFRC522_ReadReg(MFRC522_REG_FIFO_LEVEL);
    if(fifoLevel & 0x80) { // FlushBuffer bit
         MFRC522_WriteReg(MFRC522_REG_FIFO_LEVEL, 0x80);
    }
	MFRC522_WriteReg(MFRC522_REG_COMIEN, irqEn | 0x80);
	MFRC522_WriteReg(MFRC522_REG_COMIRQ, 0x7F); // Clear Interrupt bits

	// Запис даних у FIFO
	for (i = 0; i < sendLen; i++) {
		MFRC522_WriteReg(MFRC522_REG_FIFO_DATA, sendData[i]);
	}

	MFRC522_WriteReg(MFRC522_REG_COMMAND, command);
	if (command == PCD_TRANSCEIVE) {
		MFRC522_WriteReg(MFRC522_REG_BIT_FRAMING, MFRC522_ReadReg(MFRC522_REG_BIT_FRAMING) | 0x80);
	}

	// Очікування. Збільшено лічильник для стабільності
	i = 20000; // Було 2000, що замало для 100MHz без затримок
	do {
		n = MFRC522_ReadReg(MFRC522_REG_COMIRQ);
		i--;
	} while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

	MFRC522_WriteReg(MFRC522_REG_BIT_FRAMING, MFRC522_ReadReg(MFRC522_REG_BIT_FRAMING) & (~0x80));

	if (i != 0) {
		if (!(MFRC522_ReadReg(MFRC522_REG_ERROR) & 0x1B)) {
			status = 1; // OK
			if (n & irqEn & 0x01) status = 0; // Timer Error

			if (command == PCD_TRANSCEIVE) {
				n = MFRC522_ReadReg(MFRC522_REG_FIFO_LEVEL);
				uint8_t lastBits = MFRC522_ReadReg(MFRC522_REG_CONTROL) & 0x07;
				if (lastBits) *backLen = (n - 1) * 8 + lastBits;
				else *backLen = n * 8;

				if (n == 0) n = 1;
				if (n > MAX_LEN_BYTES) n = MAX_LEN_BYTES;

				for (i = 0; i < n; i++) {
					backData[i] = MFRC522_ReadReg(MFRC522_REG_FIFO_DATA);
				}
			}
		}
	}
	return status;
}

uint8_t MFRC522_Request(uint8_t reqMode, uint8_t *tagType) {
	uint8_t status;
	uint16_t backBits;
	MFRC522_WriteReg(MFRC522_REG_BIT_FRAMING, 0x07);
	tagType[0] = reqMode;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, tagType, 1, tagType, &backBits);
	if ((status != 1) || (backBits != 0x10)) status = 0;
	return status;
}

uint8_t MFRC522_Anticoll(uint8_t *serNum) {
	uint8_t status;
	uint8_t i;
	uint8_t serNumCheck = 0;
	uint16_t unLen;

	MFRC522_WriteReg(MFRC522_REG_BIT_FRAMING, 0x00);
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522_ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

	if (status == 1) {
		// Check Checksum
		for (i = 0; i < 4; i++) serNumCheck ^= serNum[i];
		if (serNumCheck != serNum[4]) status = 0;
	}
	return status;
}

void MFRC522_HardReset()
{
	volatile uint32_t count = 200000;
	GPIO_WriteToOutputPin(GPIOPins.pGPIOx, RST_PinPA10, RESET);
	while(count--);
	GPIO_WriteToOutputPin(GPIOPins.pGPIOx, RST_PinPA10, SET);
	count = 50000;
	while(count--);
}

// --- Setup Functions ---
void GPIO_Initialization()
{
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

	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = NSS_PinPB12;
	GPIO_Init(&SPIPins);

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
    // Використовуємо DIV16 для надійності (MFRC522 макс 10МГц)
	SPI2Handler.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV16;

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
    USART2Handler.pUSARTx->USART_BRR = 0x008B;
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
