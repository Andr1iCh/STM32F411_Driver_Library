/*
 * stm32f411xx_spi_driver.c
 *
 *  Created on: Dec 12, 2025
 *      Author: Andy
 */
#include "stm32f411xx_spi_driver.h"

/*
 * Helper functions prototypes
 */
static void SPI_TXE_InterruptHandle(SPI_Handle_t *pSPIHandle);
static void SPI_RXNE_InterruptHandle(SPI_Handle_t *pSPIHandle);
static void SPI_OVR_ErrInterruptHandle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
				if(pSPIx == SPI1)
				{
					SPI1_PCLK_EN();
				}else if (pSPIx == SPI2)
				{
					SPI2_PCLK_EN();
				}else if (pSPIx == SPI3)
				{
					SPI3_PCLK_EN();
				}else if (pSPIx == SPI4)
				{
					SPI4_PCLK_EN();
				}else if (pSPIx == SPI5)
				{
					SPI5_PCLK_EN();
				}
		}
	else
			{
				if(pSPIx == SPI1)
				{
					SPI1_PCLK_DI();
				}else if (pSPIx == SPI2)
				{
					SPI2_PCLK_DI();
				}else if (pSPIx == SPI3)
				{
					SPI3_PCLK_DI();
				}else if (pSPIx == SPI4)
				{
					SPI4_PCLK_DI();
				}else if (pSPIx == SPI5)
				{
					SPI5_PCLK_DI();
				}
			}

}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_Init (SPI_Handle_t *pSPIHandle)
{
	//enable peripheral clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);
	//configure SPI_CR1 register
	uint32_t tempreg = 0;

	//1. device mode
	tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	//2. Configure the bus config
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);

	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDI mode should be set
		tempreg |= (1<<SPI_CR1_BIDIMODE);

	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		//BIDI mode should be cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
		//RXONLY bit must be set
		tempreg |= (1<<SPI_CR1_RXONLY);

	}

		// 3. Configure the spi serial clock speed (baud rate)
		tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

		//4.  Configure the DFF
		tempreg |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF;

		//5. configure the CPOL
		tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

		//6 . configure the CPHA
		tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

		//7 . configure the SMM
		tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

		pSPIHandle->pSPIx->SPI_CR1 = tempreg;
}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_DeInit (SPI_Handle_t *pSPIHandle)
{
	// Check which SPI peripheral the handle is pointing to
	    if(pSPIHandle->pSPIx == SPI1)
	    {
	        SPI1_REG_RESET();
	    }
	    else if(pSPIHandle->pSPIx == SPI2)
	    {
	        SPI2_REG_RESET();
	    }
	    else if(pSPIHandle->pSPIx == SPI3)
	    {
	        SPI3_REG_RESET();
	    }
	    else if(pSPIHandle->pSPIx == SPI4)
		{
			SPI4_REG_RESET();
		}
	    else if(pSPIHandle->pSPIx == SPI5)
		{
			SPI5_REG_RESET();
		}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SPI_SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - This is blocking call

 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until TXE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG)==FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->SPI_CR1 &(1<<SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. Load the data in the DR
			pSPIx->SPI_DR = *((uint16_t*)pTxBuffer);
			Len-=2;
			pTxBuffer += 2;
		}
		else
		{
			//8 bit DFF
			pSPIx->SPI_DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - SPI_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->SPI_CR1|=(1<<SPI_CR1_SPE);
	}
	else
	{
		pSPIx->SPI_CR1&=~(1<<SPI_CR1_SPE);
	}

}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->SPI_CR1|=(1<<SPI_CR1_SSI);
	}
	else
	{
		pSPIx->SPI_CR1&=~(1<<SPI_CR1_SSI);
	}
}

/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi==ENABLE)
	{
		pSPIx->SPI_CR2|=(1<<SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->SPI_CR2&=~(1<<SPI_CR2_SSOE);
	}
}

/*********************************************************************
 * @fn      		  - SPI_ReceiveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. wait until RXNE is set
		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG)==FLAG_RESET);

		//2. check the DFF bit in CR1
		if((pSPIx->SPI_CR1 &(1<<SPI_CR1_DFF)))
		{
			//16 bit DFF
			//1. Load the data from Dr to Rxbuffer
			*((uint16_t*)pRxBuffer)= pSPIx->SPI_DR;
			Len-=2;
			pRxBuffer += 2;
		}
		else
		{
			//8 bit DFF
			*(pRxBuffer)= pSPIx->SPI_DR;
			Len--;
			pRxBuffer++;
		}
	}

}

/*
 * IRQ Configuration and ISR handling
 */
/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(IRQNumber <= 31)
			{
				//program ISER0 register
				*NVIC_ISER0 = ( 1 << IRQNumber );

			}else if(IRQNumber > 31 && IRQNumber < 64 )
			{
				//program ISER1 register (32 to 63)
				*NVIC_ISER1 = ( 1 << (IRQNumber % 32) );
			}
			else if(IRQNumber >= 64 && IRQNumber < 96 )
			{
				//program ISER2 register (64 to 95)
				*NVIC_ISER2 = ( 1 << (IRQNumber % 64) );
			}
		}else
		{
			if(IRQNumber <= 31)
			{
				//program ICER0 register
				*NVIC_ICER0 = ( 1 << IRQNumber );
			}else if(IRQNumber > 31 && IRQNumber < 64 )
			{
				//program ICER1 register
				*NVIC_ICER1 = ( 1 << (IRQNumber % 32) );
			}
			else if(IRQNumber >= 64 && IRQNumber < 96 )
			{
				//program ICER2 register
				*NVIC_ICER2 = ( 1 << (IRQNumber % 64) );
			}
		}
}

/*********************************************************************
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

/*********************************************************************
 * @fn      		  - SPI_SendDataInterruptMode
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
uint8_t SPI_SendDataInterruptMode(SPI_Handle_t *pSPIHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;

	if(state!=SPI_BUSY_IN_TX)
	{
		// Save Tx buffer address and length information in global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		// Mark SPI state as busy
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//Enable TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2|=(1<<SPI_CR2_TXEIE);
	}

	return state;

}

/*********************************************************************
 * @fn      		  - SPI_ReceiveDataInterruptMode
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
uint8_t SPI_ReceiveDataInterruptMode(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;

	if(state!=SPI_BUSY_IN_RX)
	{
		// Save Tx buffer address and length information in global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		// Mark SPI state as busy
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//Enable TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2|=(1<<SPI_CR2_RXNEIE);
	}

	return state;
}

/*********************************************************************
 * @fn      		  - SPI_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1, temp2;

	//	check for TXE
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		//handle TXE
		SPI_TXE_InterruptHandle(pSPIHandle);
	}

	//	check for RXE
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		//handle RXE
		SPI_RXNE_InterruptHandle(pSPIHandle);
	}

	//check for OVR flag
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		//handle OVR
		SPI_OVR_ErrInterruptHandle(pSPIHandle);
	}
}

/*********************************************************************
 * @fn      		  - SPI_TXE_InterruptHandle
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
static void SPI_TXE_InterruptHandle(SPI_Handle_t *pSPIHandle)
{
	if((pSPIHandle->pSPIx->SPI_CR1 &(1<<SPI_CR1_DFF)))
	{
		//16 bit DFF
		//1. Load the data in the DR
		pSPIHandle->pSPIx->SPI_DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen-=2;
		pSPIHandle->pTxBuffer += 2;
	}
	else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->SPI_DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(!pSPIHandle->TxLen)
	{
		//TX is over
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
}

/*********************************************************************
 * @fn      		  - SPI_RXNE_InterruptHandle
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
static void SPI_RXNE_InterruptHandle(SPI_Handle_t *pSPIHandle)
{
	if((pSPIHandle->pSPIx->SPI_CR1 &(1<<SPI_CR1_DFF)))
	{
		//16 bit DFF
		*((uint16_t*)pSPIHandle->pRxBuffer)= pSPIHandle->pSPIx->SPI_DR;
		pSPIHandle->RxLen-=2;
		pSPIHandle->pRxBuffer += 2;
	}
	else
	{
		//8 bit DFF
		*(pSPIHandle->pRxBuffer)= pSPIHandle->pSPIx->SPI_DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen)
	{
		//RX is over
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}
}

/*********************************************************************
 * @fn      		  - SPI_OVR_Err_InterruptHandle
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
static void SPI_OVR_ErrInterruptHandle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		SPI_ClearOVRFlag(pSPIHandle);
	}

	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}

/*********************************************************************
 * @fn      		  - SPI_CloseTransmission
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1<<SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer =NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

/*********************************************************************
 * @fn      		  - SPI_CloseReception
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1<<SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer=NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}


/*********************************************************************
 * @fn      		  - SPI_ClearOVRFlag
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note

 */
void SPI_ClearOVRFlag(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;
	temp = pSPIHandle->pSPIx->SPI_DR;
	temp = pSPIHandle->pSPIx->SPI_SR;
	(void) temp;
}

/*****************************************************************
 * @fn          - SPI_ApplicationEventCallback
 *
 * @brief       - Application event callback function
 *
 * @param[in]   - Handle structure
 * @param[in]   - Application event
 *
 * @return      - None
 *
 * @Note        - None
 *
 *****************************************************************/
__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEvent)
{

}

