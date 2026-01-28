#include "stm32f411xx.h"
#include <string.h>
#include <stdio.h>
/*
 * SPI2:
 * PB10 = CLK
 * PB12 = NSS
 * PB14	= MISO
 * PB15 = MOSI
 * alternate mode 5
 */

//command codes
#define COMMAND_LED_CTRL 	0x50
#define COMMAND_SENSOR_READ 0x51
#define COMMAND_LED_READ 	0x52
#define COMMAND_PRINT 		0x53
#define COMMAND_ID_READ 	0x54

#define LED_ON	1
#define LED_OFF	0

//arduino analog pins
#define ANALOG_PIN0		0
#define ANALOG_PIN1		1
#define ANALOG_PIN2		2
#define ANALOG_PIN3		3
#define ANALOG_PIN4		4

//arduino led
#define LED_PIN	9

uint8_t SPI_VerifyResponse(uint8_t ackByte)
{
	if(ackByte == 0xF5)
	{
		//ack
		return 1;
	}

	return 0;
}

void delay()
{
	for (uint32_t i = 0; i<500000/2; i++ );
}

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

	//MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits()
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_CPHA =	SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;

	SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit()
{
	GPIO_Handle_t GpioButton;
			GpioButton.pGPIOx = GPIOC;
			GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
			GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
			GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
			GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
		GPIO_Init(&GpioButton);
}

int main()
{


	GPIO_ButtonInit();

	//this function is used to initialize the GPIO pins to behave as SPI2 pins
	SPI2_GPIOInits();

	//This function is used to initialize the SPI2 peripheral parameters
	SPI2_Inits();

	//enable SSOE
	SPI_SSOEConfig(SPI2, ENABLE);


	while(1)
	{
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

			delay();

			// enable SPI2 peripheral
			SPI_PeripheralControl(SPI2, ENABLE);

			uint8_t dummyRead;
			uint8_t dummyWrite = 0xFF;
			uint8_t ackByte;
			uint8_t args[2];

	//1. CMD_LED_CTRL  	<pin no(1)>     <value(1)>

			uint8_t commandCode = COMMAND_LED_CTRL;

			SPI_SendData(SPI2,&commandCode, 1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummyRead, 1);

			//send a dummy byte to get answer from shift register
			SPI_SendData(SPI2, &dummyWrite, 1);

			SPI_ReceiveData(SPI2, &ackByte, 1);

			if(SPI_VerifyResponse(ackByte))
			{
				//send arguments
				args[0] = LED_PIN;
				args[1] = LED_ON;
				SPI_SendData(SPI2, &args[0], 1);
				SPI_ReceiveData(SPI2, &dummyRead, 1);
				SPI_SendData(SPI2, &args[1], 1);
				SPI_ReceiveData(SPI2, &dummyRead, 1);
				printf("COMMAND LED executed!\n");
			}
	//END of CMD_LED_CTRL

	//2. CMD_SENSOR_READ <analog pin number(1)>
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
			delay();

			commandCode = COMMAND_SENSOR_READ;

			SPI_SendData(SPI2,&commandCode, 1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummyRead, 1);

			//send a dummy byte to get answer from shift register
			SPI_SendData(SPI2, &dummyWrite, 1);

			SPI_ReceiveData(SPI2, &ackByte, 1);


			if(SPI_VerifyResponse(ackByte))
			{
				//send arguments
				args[0] = ANALOG_PIN0;
				SPI_SendData(SPI2, args, 1);

				SPI_ReceiveData(SPI2, &dummyRead, 1);
				SPI_SendData(SPI2, &dummyWrite, 1);

				//delay to get some time for analog read
				delay();

				uint8_t analog_read;
				SPI_ReceiveData(SPI2, &analog_read, 1);
				 printf("Analog read value: %d\n",analog_read);
			}
	//END of CMD_SENSOR_READ

	//3. COMMAND_LED_READ <pin number(1)>
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
			delay();
			commandCode = COMMAND_LED_READ;

			SPI_SendData(SPI2,&commandCode, 1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummyRead, 1);

			//send a dummy byte to get answer from shift register
			SPI_SendData(SPI2, &dummyWrite, 1);

			SPI_ReceiveData(SPI2, &ackByte, 1);


			if(SPI_VerifyResponse(ackByte))
			{
				//send arguments
				args[0] = LED_PIN;
				SPI_SendData(SPI2, args, 1);

				SPI_ReceiveData(SPI2, &dummyRead, 1);
				SPI_SendData(SPI2, &dummyWrite, 1);

				//delay to get some time for analog read
				delay();

				uint8_t led_status;
				SPI_ReceiveData(SPI2, &led_status, 1);
				printf("Read LED: %d\n",led_status);
			}
	//END of COMMAND_LED_READ

	//4. COMMAND_PRINT <len(1)> <message(len)>
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
			commandCode = COMMAND_PRINT;

			SPI_SendData(SPI2,&commandCode, 1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummyRead, 1);

			//send a dummy byte to get answer from shift register
			SPI_SendData(SPI2, &dummyWrite, 1);

			SPI_ReceiveData(SPI2, &ackByte, 1);

			uint8_t message[] = "Hello ! How are you ??";

			if(SPI_VerifyResponse(ackByte))
			{
				//send arguments
				args[0] = strlen((char*)message);
				SPI_SendData(SPI2, args, 1);

				 //dummy read to clear off the RXNE
				SPI_ReceiveData(SPI2, &dummyRead, 1);

				//wait for Slave to be ready with data
				delay();

				//send message
				for(int i = 0 ; i < args[0] ; i++){
					SPI_SendData(SPI2,&message[i],1);
					SPI_ReceiveData(SPI2,&dummyRead,1);
				}

				printf("COMMAND Print executed!\n");
			}
	//END of COMMAND_PRINT

	//5. COMMAND_ID_READ <pin number(1)>
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
			commandCode = COMMAND_ID_READ;

			SPI_SendData(SPI2,&commandCode, 1);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummyRead, 1);

			//send a dummy byte to get answer from shift register
			SPI_SendData(SPI2, &dummyWrite, 1);

			SPI_ReceiveData(SPI2, &ackByte, 1);

			uint8_t id[11];
			uint32_t i=0;

			if(SPI_VerifyResponse(ackByte))
			{
				// Read 10 bytes id from the slave
				for(  i = 0 ; i < 10 ; i++){
				// Send dummy bits (byte) to fetch the response from slave
				SPI_SendData(SPI2,&dummyWrite,1);
				SPI_ReceiveData(SPI2,&id[i],1);
			}
				id[10] = '\0';

				printf("COMMAND ID : %s \n",id);
			}
	//END of COMMAND_ID_READ

			//confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

			// disable SPI2 peripheral
			SPI_PeripheralControl(SPI2, DISABLE);

	}

	return 0;
}
