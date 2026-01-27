/*
 * stm32f411xx.h
 *
 *  Created on: Oct 22, 2025
 *      Author: Andy
 */

#ifndef INC_STM32F411XX_H_
#define INC_STM32F411XX_H_

#include<stddef.h>
#include<stdint.h>

#define __vo volatile
#define __weak __attribute__((weak))

/**********************************START:Processor Specific Details **********************************/
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 */

#define NVIC_ISER0          ( (__vo uint32_t*)0xE000E100 )
#define NVIC_ISER1          ( (__vo uint32_t*)0xE000E104 )
#define NVIC_ISER2          ( (__vo uint32_t*)0xE000E108 )
#define NVIC_ISER3          ( (__vo uint32_t*)0xE000E10c )


/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 */
#define NVIC_ICER0 			((__vo uint32_t*)0XE000E180)
#define NVIC_ICER1			((__vo uint32_t*)0XE000E184)
#define NVIC_ICER2  		((__vo uint32_t*)0XE000E188)
#define NVIC_ICER3			((__vo uint32_t*)0XE000E18C)


/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR 	((__vo uint32_t*)0xE000E400)

/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED  4

/*
 * base addresses of Flash and SRAM memories
 */

#define FLASH_BASEADDR						0x08000000U   		/*!<explain this macro briefly here  */
#define SRAM_BASEADDR						0x20000000U  		/*!<explain this macro briefly here  */
#define ROM_BASEADDR						0x1FFF0000U
#define SRAM 								SRAM_BASEADDR


/*
 * AHBx and APBx Bus Peripheral base addresses
 */

#define PERIPH_BASEADDR 						0x40000000U
#define APB1PERIPH_BASEADDR						PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR						0x40010000U
#define AHB1PERIPH_BASEADDR						0x40020000U
#define AHB2PERIPH_BASEADDR						0x50000000U

/*
 * Base addresses of peripherals which are hanging on AHB1 bus
 */
#define GPIOA_BASEADDR                   (AHB1PERIPH_BASEADDR + 0x0000)
#define GPIOB_BASEADDR                   (AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR 					 (AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR 					 (AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR 					 (AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOH_BASEADDR 					 (AHB1PERIPH_BASEADDR + 0x1C00)

#define CRC_BASEADDR 					 (AHB1PERIPH_BASEADDR + 0x3000)
#define RCC_BASEADDR                     (AHB1PERIPH_BASEADDR + 0x3800)

/*
 * Base addresses of peripherals which are hanging on APB1 bus
 */
#define I2C1_BASEADDR						(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR						(APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASEADDR						(APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASEADDR						(APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASEADDR						(APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASEADDR						(APB1PERIPH_BASEADDR + 0x4400)

/*
 * Base addresses of peripherals which are hanging on APB2 bus
 */
#define EXTI_BASEADDR						(APB2PERIPH_BASEADDR + 0x3C00)

#define SPI1_BASEADDR						(APB2PERIPH_BASEADDR + 0x3000)
#define SPI4_BASEADDR						(APB2PERIPH_BASEADDR + 0x3400)
#define SPI5_BASEADDR						(APB2PERIPH_BASEADDR + 0x5000)

#define SYSCFG_BASEADDR        				(APB2PERIPH_BASEADDR + 0x3800)
#define USART1_BASEADDR						(APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASEADDR						(APB2PERIPH_BASEADDR + 0x1400)





/**********************************peripheral register definition structures **********************************/

/*
 * Note : Registers of a peripheral are specific to MCU
 * e.g : Number of Registers of SPI peripheral of STM32F4x family of MCUs may be different(more or less)
 * Compared to number of registers of SPI peripheral of STM32Lx or STM32F0x family of MCUs
 * Please check your Device RM
 */

typedef struct
{
	__vo uint32_t MODER;                        /*!< GPIO port mode register,                    	Address offset: 0x00      */
	__vo uint32_t OTYPER;                       /*!< TODO,     										Address offset: 0x04      */
	__vo uint32_t OSPEEDR;
	__vo uint32_t PUPDR;
	__vo uint32_t IDR;
	__vo uint32_t ODR;
	__vo uint32_t BSRR;
	__vo uint32_t LCKR;
	__vo uint32_t AFRL;
	__vo uint32_t AFRH;
}GPIO_RegDef_t;


/*
 * peripheral register definition structure for RCC
 */
typedef struct
{
  __vo uint32_t CR;            /*!< TODO,     										Address offset: 0x00 */
  __vo uint32_t PLLCFGR;       /*!< TODO,     										Address offset: 0x04 */
  __vo uint32_t CFGR;          /*!< TODO,     										Address offset: 0x08 */
  __vo uint32_t CIR;           /*!< TODO,     										Address offset: 0x0C */
  __vo uint32_t AHB1RSTR;      /*!< TODO,     										Address offset: 0x10 */
  __vo uint32_t AHB2RSTR;      /*!< TODO,     										Address offset: 0x14 */
  uint32_t      RESERVED0[2];  /*!< Reserved, 0x18-0x1C                                                  */
  __vo uint32_t APB1RSTR;      /*!< TODO,     										Address offset: 0x20 */
  __vo uint32_t APB2RSTR;      /*!< TODO,     										Address offset: 0x24 */
  uint32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                  */
  __vo uint32_t AHB1ENR;       /*!< TODO,     										Address offset: 0x30 */
  __vo uint32_t AHB2ENR;       /*!< TODO,     										Address offset: 0x34 */
  uint32_t      RESERVED2[2];  /*!< Reserved, 0x38-0x3C                                                  */
  __vo uint32_t APB1ENR;       /*!< TODO,     										Address offset: 0x40 */
  __vo uint32_t APB2ENR;       /*!< TODO,     										Address offset: 0x44 */
  uint32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                  */
  __vo uint32_t AHB1LPENR;     /*!< TODO,     										Address offset: 0x50 */
  __vo uint32_t AHB2LPENR;     /*!< TODO,     										Address offset: 0x54 */
  uint32_t      RESERVED4[2];  /*!< Reserved, 0x58-0x5C                                                  */
  __vo uint32_t APB1LPENR;     /*!< TODO,     										Address offset: 0x60 */
  __vo uint32_t APB2LPENR;     /*!< RTODO,     										Address offset: 0x64 */
  uint32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                  */
  __vo uint32_t BDCR;          /*!< TODO,     										Address offset: 0x70 */
  __vo uint32_t CSR;           /*!< TODO,     										Address offset: 0x74 */
  uint32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                  */
  __vo uint32_t SSCGR;         /*!< TODO,     										Address offset: 0x80 */
  __vo uint32_t PLLI2SCFGR;    /*!< TODO,     										Address offset: 0x84 */
  uint32_t      RESERVED7;     /*!< Reserved, 0x88                                                       */
  __vo uint32_t DCKCFGR;       /*!< TODO,     										Address offset: 0x8C */
} RCC_RegDef_t;


/*
 * peripheral register definition structure for EXTI
 */
typedef struct
{
	__vo uint32_t IMR;    /*!< Give a short description,          	  	    Address offset: 0x00 */
	__vo uint32_t EMR;    /*!< TODO,                						Address offset: 0x04 */
	__vo uint32_t RTSR;   /*!< TODO,  									     Address offset: 0x08 */
	__vo uint32_t FTSR;   /*!< TODO, 										Address offset: 0x0C */
	__vo uint32_t SWIER;  /*!< TODO,  									   Address offset: 0x10 */
	__vo uint32_t PR;     /*!< TODO,                   					   Address offset: 0x14 */

}EXTI_RegDef_t;

/*
 * peripheral register definition structure for SYSCFG
 */
typedef struct
{
	__vo uint32_t MEMRMP;       /*!< 							                  Address offset: 0x00      */
	__vo uint32_t PMC;          /*!< TODO,     									  Address offset: 0x04      */
	__vo uint32_t EXTICR[4];    /*!< TODO , 									  Address offset: 0x08-0x14 */
	uint32_t      RESERVED1[2];  /*!< TODO          							  Reserved, 0x18-0x1C    	*/
	__vo uint32_t CMPCR;        /*!< TODO         								  Address offset: 0x20      */
	uint32_t      RESERVED2[2];  /*!<                                             Reserved, 0x24-0x28 	    */
	__vo uint32_t CFGR;         /*!< TODO                                         Address offset: 0x2C   	*/
} SYSCFG_RegDef_t;

/*
 * peripheral register definition structure for SPI
 */
typedef struct
{
	__vo uint32_t SPI_CR1;    /*!<												Address offset: 0x00 */
	__vo uint32_t SPI_CR2;    /*!< TODO,                						Address offset: 0x04 */
	__vo uint32_t SPI_SR;   /*!< TODO,  									    Address offset: 0x08 */
	__vo uint32_t SPI_DR;   /*!< TODO, 											Address offset: 0x0C */
	__vo uint32_t SPI_CRCPR;  /*!< TODO,  										Address offset: 0x10 */
	__vo uint32_t SPI_RXCRCR;     /*!< TODO,                   					Address offset: 0x14 */
	__vo uint32_t SPI_TXCRCR;     /*!< TODO,                   			  		Address offset: 0x18 */
	__vo uint32_t SPI_I2SCFGR;     /*!< TODO,                   				Address offset: 0x1C */
	__vo uint32_t SPI_I2SPR;     /*!< TODO,                   					Address offset: 0x20 */

}SPI_RegDef_t;

/*
 * peripheral register definition structure for USART
 */
typedef struct
{
	__vo uint32_t USART_SR;    /*!<												Address offset: 0x00 */
	__vo uint32_t USART_DR;    /*!< TODO,                						Address offset: 0x04 */
	__vo uint32_t USART_BRR;   /*!< TODO,  									    Address offset: 0x08 */
	__vo uint32_t USART_CR1;   /*!< TODO, 											Address offset: 0x0C */
	__vo uint32_t USART_CR2;  /*!< TODO,  										Address offset: 0x10 */
	__vo uint32_t USART_CR3;     /*!< TODO,                   					Address offset: 0x14 */
	__vo uint32_t USART_GTPR;     /*!< TODO,                   			  		Address offset: 0x18 */
}USART_RegDef_t;

/*
 * peripheral definitions (Peripheral base addresses typecasted to xxx_RegDef_t)
 */
#define GPIOA        ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB        ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC        ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD        ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE        ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOH        ((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC          ((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI  		 ((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG		 ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

#define SPI1			((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2    		((SPI_RegDef_t*)SPI2_BASEADDR)
#define SPI3			((SPI_RegDef_t*)SPI3_BASEADDR)
#define SPI4			((SPI_RegDef_t*)SPI4_BASEADDR)
#define SPI5			((SPI_RegDef_t*)SPI5_BASEADDR)

#define USART1			((USART_RegDef_t*)USART1_BASEADDR)
#define USART2			((USART_RegDef_t*)USART2_BASEADDR)
#define USART6			((USART_RegDef_t*)USART6_BASEADDR)

/*
 * Clock Enable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_EN()  (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()  (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()  (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()  (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()  (RCC->AHB1ENR |= (1 << 4))
#define GPIOH_PCLK_EN()  (RCC->AHB1ENR |= (1 << 7))

/*
 * Clock Enable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_EN()   (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()   (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()   (RCC->APB1ENR |= (1 << 23))

/*
 * Clock Enable Macros for SPIx peripherals
 */
#define SPI1_PCLK_EN()  (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()  (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()  (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()  (RCC->APB2ENR |= (1 << 13))
#define SPI5_PCLK_EN()  (RCC->APB2ENR |= (1 << 20))

/*
 * Clock Enable Macros for USARTx peripherals
 */
#define USART1_PCLK_EN()  (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()  (RCC->APB1ENR |= (1 << 17))
#define USART6_PCLK_EN()  (RCC->APB2ENR |= (1 << 5))

/*
 * Clock Enable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_EN() (RCC->APB2ENR |= (1 << 14))

/*
 * Clock Disable Macros for GPIOx peripherals
 */
#define GPIOA_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOH_PCLK_DI()  (RCC->AHB1ENR &= ~(1 << 7))

/*
 * Clock Disable Macros for I2Cx peripherals
 */
#define I2C1_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 23))


/*
 * Clock Disable Macros for SPIx peripherals
 */
#define SPI1_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 13))
#define SPI5_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 20))

/*
 * Clock Disable Macros for USARTx peripherals
 */
#define USART1_PCCK_DI()  (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCCK_DI()  (RCC->APB1ENR &= ~(1 << 17))
#define USART6_PCCK_DI()  (RCC->APB2ENR &= ~(1 << 5))


/*
 * Clock Disable Macros for SYSCFG peripheral
 */
#define SYSCFG_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 14))


/*
 * Clock Disable Macros for SPIx peripherals
 */

/*
 * Clock Disable Macros for USARTx peripherals
 */


/*
 * Clock Disable Macros for SYSCFG peripheral
 */


/*
 *  Macros to reset GPIOx peripherals
 */
#define GPIOA_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOH_REG_RESET()               do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)

/*
 *  Macros to reset SPIx peripherals
 */
#define SPI1_REG_RESET()  				do{ (RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12)); }while(0)
#define SPI2_REG_RESET()  				do{ (RCC->APB1RSTR |= (1 << 14)); (RCC->APB1RSTR &= ~(1 << 14)); }while(0)
#define SPI3_REG_RESET()  				do{ (RCC->APB1RSTR |= (1 << 15)); (RCC->APB1RSTR &= ~(1 << 15)); }while(0)
#define SPI4_REG_RESET()  				do{ (RCC->APB2RSTR |= (1 << 13)); (RCC->APB2RSTR &= ~(1 << 13)); }while(0)
#define SPI5_REG_RESET()  				do{ (RCC->APB2RSTR |= (1 << 20)); (RCC->APB2RSTR &= ~(1 << 20)); }while(0)


/*
 * This macro returns a code( between 0 to 7, 5-6 are reserved) for a given GPIO base address(x)
 */
#define GPIO_BASEADDR_TO_CODE(x)      ((x == GPIOA)?0:\
										(x == GPIOB)?1:\
										(x == GPIOC)?2:\
										(x == GPIOD)?3:\
								        (x == GPIOE)?4:\
								        (x == GPIOH)?7:0)

/*
 * IRQ(Interrupt Request) Numbers of STM32F411x MCU
 */

#define IRQ_NO_WWDG                    0   /* Window Watchdog interrupt                                         */
#define IRQ_NO_EXTI16_PVD              1   /* EXTI Line 16 interrupt / PVD through EXTI line detection          */
#define IRQ_NO_EXTI21_TAMP_STAMP       2   /* EXTI Line 21 interrupt / Tamper and TimeStamp interrupts          */
#define IRQ_NO_EXTI22_RTC_WKUP         3   /* EXTI Line 22 interrupt / RTC Wake-up interrupt                    */
#define IRQ_NO_FLASH                   4   /* Flash global interrupt                                            */
#define IRQ_NO_RCC                     5   /* RCC global interrupt                                              */
#define IRQ_NO_EXTI0                   6   /* EXTI Line0 interrupt                                              */
#define IRQ_NO_EXTI1                   7   /* EXTI Line1 interrupt                                              */
#define IRQ_NO_EXTI2                   8   /* EXTI Line2 interrupt                                              */
#define IRQ_NO_EXTI3                   9   /* EXTI Line3 interrupt                                              */
#define IRQ_NO_EXTI4                   10  /* EXTI Line4 interrupt                                              */
#define IRQ_NO_DMA1_Stream0            11  /* DMA1 Stream0 global interrupt                                     */
#define IRQ_NO_DMA1_Stream1            12  /* DMA1 Stream1 global interrupt                                     */
#define IRQ_NO_DMA1_Stream2            13  /* DMA1 Stream2 global interrupt                                     */
#define IRQ_NO_DMA1_Stream3            14  /* DMA1 Stream3 global interrupt                                     */
#define IRQ_NO_DMA1_Stream4            15  /* DMA1 Stream4 global interrupt                                     */
#define IRQ_NO_DMA1_Stream5            16  /* DMA1 Stream5 global interrupt                                     */
#define IRQ_NO_DMA1_Stream6            17  /* DMA1 Stream6 global interrupt                                     */
#define IRQ_NO_ADC                     18  /* ADC1 global interrupts                                            */
#define IRQ_NO_EXTI9_5                 23  /* EXTI Line[9:5] interrupts                                         */
#define IRQ_NO_TIM1_BRK_TIM9           24  /* TIM1 Break interrupt and TIM9 global interrupt                   */
#define IRQ_NO_TIM1_UP_TIM10           25  /* TIM1 Update interrupt and TIM10 global interrupt                  */
#define IRQ_NO_TIM1_TRG_COM_TIM11      26  /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt*/
#define IRQ_NO_TIM1_CC                 27  /* TIM1 Capture Compare interrupt                                    */
#define IRQ_NO_TIM2                    28  /* TIM2 global interrupt                                             */
#define IRQ_NO_TIM3                    29  /* TIM3 global interrupt                                             */
#define IRQ_NO_TIM4                    30  /* TIM4 global interrupt                                             */
#define IRQ_NO_I2C1_EV                 31  /* I2C1 event interrupt                                              */
#define IRQ_NO_I2C1_ER                 32  /* I2C1 error interrupt                                              */
#define IRQ_NO_I2C2_EV                 33  /* I2C2 event interrupt                                              */
#define IRQ_NO_I2C2_ER                 34  /* I2C2 error interrupt                                              */
#define IRQ_NO_SPI1                    35  /* SPI1 global interrupt                                             */
#define IRQ_NO_SPI2                    36  /* SPI2 global interrupt                                             */
#define IRQ_NO_USART1                  37  /* USART1 global interrupt                                           */
#define IRQ_NO_USART2                  38  /* USART2 global interrupt                                           */
#define IRQ_NO_EXTI15_10               40  /* EXTI Line[15:10] interrupts                                       */
#define IRQ_NO_EXTI17_RTC_Alarm        41  /* EXTI Line 17 interrupt / RTC Alarms (A and B)                     */
#define IRQ_NO_EXTI18_OTG_FS_WKUP      42  /* EXTI Line 18 interrupt / USB On-The-Go FS Wake-up                 */
#define IRQ_NO_DMA1_Stream7            47  /* DMA1 Stream7 global interrupt                                     */
#define IRQ_NO_SDIO                    49  /* SDIO global interrupt                                             */
#define IRQ_NO_TIM5                    50  /* TIM5 global interrupt                                             */
#define IRQ_NO_SPI3                    51  /* SPI3 global interrupt                                             */
#define IRQ_NO_DMA2_Stream0            56  /* DMA2 Stream0 global interrupt                                     */
#define IRQ_NO_DMA2_Stream1            57  /* DMA2 Stream1 global interrupt                                     */
#define IRQ_NO_DMA2_Stream2            58  /* DMA2 Stream2 global interrupt                                     */
#define IRQ_NO_DMA2_Stream3            59  /* DMA2 Stream3 global interrupt                                     */
#define IRQ_NO_DMA2_Stream4            60  /* DMA2 Stream4 global interrupt                                     */
#define IRQ_NO_OTG_FS                  67  /* USB On The Go FS global interrupt                                 */
#define IRQ_NO_DMA2_Stream5            68  /* DMA2 Stream5 global interrupt                                     */
#define IRQ_NO_DMA2_Stream6            69  /* DMA2 Stream6 global interrupt                                     */
#define IRQ_NO_DMA2_Stream7            70  /* DMA2 Stream7 global interrupt                                     */
#define IRQ_NO_USART6                  71  /* USART6 global interrupt                                           */
#define IRQ_NO_I2C3_EV                 72  /* I2C3 event interrupt                                              */
#define IRQ_NO_I2C3_ER                 73  /* I2C3 error interrupt                                              */
#define IRQ_NO_FPU                     81  /* FPU global interrupt                                              */
#define IRQ_NO_SPI4                    84  /* SPI 4 global interrupt                                            */
#define IRQ_NO_SPI5                    85  /* SPI 5 global interrupt											*/

/*
 * macros for all the possible priority levels
 */

#define NVIC_IRQ_PRI0       0
#define NVIC_IRQ_PRI1       1
#define NVIC_IRQ_PRI2       2
#define NVIC_IRQ_PRI3       3
#define NVIC_IRQ_PRI4       4
#define NVIC_IRQ_PRI5       5
#define NVIC_IRQ_PRI6       6
#define NVIC_IRQ_PRI7       7
#define NVIC_IRQ_PRI8       8
#define NVIC_IRQ_PRI9       9
#define NVIC_IRQ_PRI10      10
#define NVIC_IRQ_PRI11      11
#define NVIC_IRQ_PRI12      12
#define NVIC_IRQ_PRI13      13
#define NVIC_IRQ_PRI14      14
#define NVIC_IRQ_PRI15      15


//some generic macros

#define ENABLE                  1
#define DISABLE                 0
#define SET                     ENABLE
#define RESET                   DISABLE
#define GPIO_PIN_SET            SET
#define GPIO_PIN_RESET          RESET
#define FLAG_SET				SET
#define FLAG_RESET				RESET

/******************************************************************************************
 *Bit position definitions of SPI peripheral
 ******************************************************************************************/
/*
 * Bit position definitions SPI_CR1
 */
#define SPI_CR1_CPHA     				 0
#define SPI_CR1_CPOL      				 1
#define SPI_CR1_MSTR     				 2
#define SPI_CR1_BR   					 3
#define SPI_CR1_SPE     				 6
#define SPI_CR1_LSBFIRST   			 	 7
#define SPI_CR1_SSI     				 8
#define SPI_CR1_SSM      				 9
#define SPI_CR1_RXONLY      		 	10
#define SPI_CR1_DFF     			 	11
#define SPI_CR1_CRCNEXT   			 	12
#define SPI_CR1_CRCEN   			 	13
#define SPI_CR1_BIDIOE     			 	14
#define SPI_CR1_BIDIMODE      			15

/*
 * Bit position definitions SPI_CR2
 */
#define SPI_CR2_RXDMAEN		 			0
#define SPI_CR2_TXDMAEN				 	1
#define SPI_CR2_SSOE				 	2
#define SPI_CR2_FRF						4
#define SPI_CR2_ERRIE					5
#define SPI_CR2_RXNEIE				 	6
#define SPI_CR2_TXEIE					7


/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE						0
#define SPI_SR_TXE				 		1
#define SPI_SR_CHSIDE				 	2
#define SPI_SR_UDR					 	3
#define SPI_SR_CRCERR				 	4
#define SPI_SR_MODF					 	5
#define SPI_SR_OVR					 	6
#define SPI_SR_BSY					 	7
#define SPI_SR_FRE					 	8

/******************************************************************************************
 *Bit position definitions of USART peripheral
 ******************************************************************************************/
/*
 * Bit position definitions USART_CR1
 */
#define USART_CR1_SBK                      0    // Send Break
#define USART_CR1_RWU                      1    // Receiver Wakeup
#define USART_CR1_RE                       2    // Receiver Enable
#define USART_CR1_TE                       3    // Transmitter Enable
#define USART_CR1_IDLEIE                   4    // IDLE Interrupt Enable
#define USART_CR1_RXNEIE                   5    // RXNE Interrupt Enable
#define USART_CR1_TCIE                     6    // Transmission Complete Interrupt Enable
#define USART_CR1_TXEIE                    7    // TXE Interrupt Enable
#define USART_CR1_PEIE                     8    // PE Interrupt Enable
#define USART_CR1_PS                       9    // Parity Selection
#define USART_CR1_PCE                      10   // Parity Control Enable
#define USART_CR1_WAKE                     11   // Wakeup method
#define USART_CR1_M                        12   // Word length (0: 8 bits, 1: 9 bits)
#define USART_CR1_UE                       13   // USART Enable
#define USART_CR1_OVER8                    15   // Oversampling mode (0: 16x, 1: 8x)

/*
 * Bit position definitions USART_CR2
 */
#define USART_CR2_ADD                      0    // Address of the USART node
#define USART_CR2_LBDL                     5    // LIN break detection length
#define USART_CR2_LBDIE                    6    // LIN break detection interrupt enable
#define USART_CR2_LBCL                     8    // Last bit clock pulse
#define USART_CR2_CPHA                     9    // Clock phase
#define USART_CR2_CPOL                     10   // Clock polarity
#define USART_CR2_CLKEN                    11   // Clock enable
#define USART_CR2_STOP                     12   // STOP bits (bits 12 & 13)
#define USART_CR2_LINEN                    14   // LIN mode enable

/*
 * Bit position definitions USART_CR3
 */
#define USART_CR3_EIE                      0    // Error interrupt enable
#define USART_CR3_IREN                     1    // IrDA mode enable
#define USART_CR3_IRLP                     2    // IrDA low-power
#define USART_CR3_HDSEL                    3    // Half-duplex selection
#define USART_CR3_NACK                     4    // Smartcard NACK enable
#define USART_CR3_SCEN                     5    // Smartcard mode enable
#define USART_CR3_DMAR                     6    // DMA enable receiver
#define USART_CR3_DMAT                     7    // DMA enable transmitter
#define USART_CR3_RTSE                     8    // Request to send enable
#define USART_CR3_CTSE                     9    // Clear to send enable
#define USART_CR3_CTSIE                    10   // CTS interrupt enable
#define USART_CR3_ONEBIT                   11   // One sample bit method enable

/*
 * Bit position definitions USART_SR
 */
#define USART_SR_PE                        0    // Parity Error
#define USART_SR_FE                        1    // Framing Error
#define USART_SR_NF                        2    // Noise Detected Flag
#define USART_SR_ORE                       3    // Overrun Error
#define USART_SR_IDLE                      4    // IDLE line detected
#define USART_SR_RXNE                      5    // Read Data Register Not Empty
#define USART_SR_TC                        6    // Transmission Complete
#define USART_SR_TXE                       7    // Transmit Data Register Empty
#define USART_SR_LBD                       8    // LIN Break Detection Flag
#define USART_SR_CTS                       9    // CTS Flag


#include "stm32f411xx_gpio_driver.h"
#include "stm32f411xx_spi_driver.h"
#include "stm32f411xx_usart_driver.h"

#endif /* INC_STM32F411XX_H_ */


