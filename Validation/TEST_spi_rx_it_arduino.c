	#include "stm32f411xx.h"

	/*
	 * stm32	- master
	 * arduino 	- slave
	 * PA12(stm) - 8 (ardu)  message signal pin
	 *
	 *The user receives messages from Arduino sent via Serial Monitor.
	 *These will be displayed in the SWV ITM Data Console.
	 */


	// SPI2 NSS = PB12
	#define NSS_PinPB12 GPIO_PIN_NO_12
	// SPI2 SCKL = PB13
	#define SCKL_PinPB13 GPIO_PIN_NO_13
	// SPI2 MISO = PB14
	#define MISO_PinPB14 GPIO_PIN_NO_14
	// SPI2 MOSI = PB15
	#define MOSI_PinPB15 GPIO_PIN_NO_15
	// Data availability signal
	#define DataAv_PinPA12 GPIO_PIN_NO_12
	#define MAX_LEN 500

	void SPI2_GPIO_Init();
	void DataAv_GPIO_Init();
	void SPI2_Init();

	SPI_Handle_t SPI2Handle;
	char RcvBuff[MAX_LEN];
	volatile char ReadByte;

	//flags
	volatile uint8_t dataAvailable;
	volatile uint8_t receiveStop;

	int main()
	{

		uint8_t dummyByte = 0xFF;

		SPI2_GPIO_Init();

		DataAv_GPIO_Init();

		SPI2_Init();

		SPI_SSOEConfig(SPI2, ENABLE);

		SPI_IRQInterruptConfig(IRQ_NO_SPI2, ENABLE);

		while(1)
		{
			receiveStop = 0;

			while(!dataAvailable);
			GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, DISABLE);

			SPI_PeripheralControl(SPI2, ENABLE);

			while(!receiveStop)
			{
				while(SPI_SendDataInterruptMode(&SPI2Handle, &dummyByte, 1)==SPI_BUSY_IN_TX);
				while(SPI_ReceiveDataInterruptMode(&SPI2Handle, &ReadByte, 1)==SPI_BUSY_IN_RX);

			}

			//confirm SPI is not busy
			while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

			SPI_PeripheralControl(SPI2, DISABLE);

			printf("Received data: %s\n", RcvBuff);
			dataAvailable = 0;

			GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);


		}




	}

	void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,uint8_t AppEv)
	{
		static uint32_t i = 0;
		if(AppEv==SPI_EVENT_RX_CMPLT)
		{
			RcvBuff[i++]= ReadByte;

			if((ReadByte == '\0')||(i == MAX_LEN))
			{
				receiveStop = 1;
				RcvBuff[i-1] = '\0';
				i = 0;
			}
		}
	}

	void EXTI15_10_IRQHandler(void)
	{
		dataAvailable = 1;
		GPIO_IRQHandling(DataAv_PinPA12);
	}

	void SPI2_IRQHandler(void)
	{
		SPI_IRQHandling(&SPI2Handle);
	}

	void SPI2_GPIO_Init()
	{
		GPIO_Handle_t SPIPins;
		SPIPins.pGPIOx = GPIOB;
		SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
		SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;

		SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
		SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
		SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

		SPIPins.GPIO_PinConfig.GPIO_PinNumber = NSS_PinPB12;
		GPIO_Init(&SPIPins);

		SPIPins.GPIO_PinConfig.GPIO_PinNumber = SCKL_PinPB13;
		GPIO_Init(&SPIPins);

		SPIPins.GPIO_PinConfig.GPIO_PinNumber = MISO_PinPB14;
		GPIO_Init(&SPIPins);

		SPIPins.GPIO_PinConfig.GPIO_PinNumber = MOSI_PinPB15;
		GPIO_Init(&SPIPins);


	}

	/*This function configures the gpio pin over which SPI peripheral issues data available interrupt */
	void DataAv_GPIO_Init()
	{
		GPIO_Handle_t DataAv_Pin;
		DataAv_Pin.pGPIOx = GPIOA;
		DataAv_Pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
		DataAv_Pin.GPIO_PinConfig.GPIO_PinNumber = DataAv_PinPA12;
		DataAv_Pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
		DataAv_Pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

		GPIO_Init(&DataAv_Pin);
		GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);
		GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

	}

	void SPI2_Init()
	{
		SPI2Handle.pSPIx = SPI2;
		SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
		SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
		SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
		SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
		SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
		SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;
		SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;

		SPI_Init(&SPI2Handle);
	}
