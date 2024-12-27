//
// Created by Phan Linh on 25/12/2024.
//

#ifndef MCUSTM32FL0X0_H
#define MCUSTM32FL0X0_H

#include <stdint.h>

#define FPU 							16000000

#define PERIPH_BASE           			0x40000000U
#define APB1_BASE           		    0x40000000U
#define APB2_BASE           		    0x40010000U
#define AHB_BASE           		        0x40020000U
#define IOPORT                          0x50000000U


// IO Port
#define GPIOA_BASE            			(IOPORT 	+ 0x0000U)
#define GPIOB_BASE            			(IOPORT 	+ 0x0400U)
#define GPIOC_BASE            			(IOPORT 	+ 0x0800U)
#define GPIOD_BASE            			(IOPORT 	+ 0x0C00U)
#define GPIOE_BASE            			(IOPORT 	+ 0x1000U)
#define GPIOH_BASE            			(IOPORT 	+ 0x1C00U)
// AHB
#define DMA1_BASE              			(AHB_BASE 	+ 0x0000U)
#define RCC_BASE              			(AHB_BASE 	+ 0x1000U)
#define FLASH_BASE              		(AHB_BASE 	+ 0x2000U)
#define CRC_BASE              		    (AHB_BASE 	+ 0x3000U)

// APB2
#define SYSCFG_BASE 		  			(APB2_BASE 	+ 0x0000U)
#define EXTI_BASE 		  				(APB2_BASE 	+ 0x0400U)
#define TIM21_BASE 		  				(APB2_BASE 	+ 0x0800U)
#define TIM22_BASE 		  				(APB2_BASE 	+ 0x0800U)
#define FIREWALL_BASE 		  			(APB2_BASE 	+ 0x1C00U)
#define ADC1_BASE 		  				(APB2_BASE 	+ 0x2400U)
#define SPI1_BASE 		  				(APB2_BASE 	+ 0x3000U)
#define DBG_BASE                        (APB2_BASE 	+ 0x5800U)

// APB1
#define TIM2_BASE 						(APB1_BASE 	+ 0x0000U)
#define RTC 						    (APB1_BASE 	+ 0x2800U)
#define WWDG_BASE 						(APB1_BASE 	+ 0x2C00U)
#define IWDG_BASE 						(APB1_BASE 	+ 0x3000U)
#define USART2_BASE                     (APB1_BASE 	+ 0x4400U)
#define LPUART1_BASE                    (APB1_BASE 	+ 0x4800U)
#define I2C1_BASE 						(APB1_BASE 	+ 0x5400U)
#define PWR_BASE 						(APB1_BASE 	+ 0x7000U)
#define LPTIM1 						    (APB1_BASE 	+ 0x7C00U)

// ARM Address
#define NVIC_ISER0_BASE					0xE000E100U
#define NVIC_ISER1_BASE					0xE000E104U
#define NVIC_ISER2_BASE					0xE000E108U
#define NVIC_ISER3_BASE					0xE000E10CU
#define NVIC_ISER4_BASE					0xE000E110U
#define NVIC_ISER5_BASE					0xE000E114U
#define NVIC_ISER6_BASE					0xE000E118U
#define NVIC_ISER7_BASE					0xE000E11CU


#define reg16( r ) unsigned short r
#define reg32( r ) unsigned long r


/******************************************************************************
*									STM32 GPIOA_BASE												*
*									General Purpose I/O											*
*******************************************************************************/
// Field map
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t MODER0:2;
		uint32_t MODER1:2;
		uint32_t MODER2:2;
		uint32_t MODER3:2;
		uint32_t MODER4:2;
		uint32_t MODER5:2;
		uint32_t MODER6:2;
		uint32_t MODER7:2;
		uint32_t MODER8:2;
		uint32_t MODER9:2;
		uint32_t MODER10:2;
		uint32_t MODER11:2;
		uint32_t MODER12:2;
		uint32_t MODER13:2;
		uint32_t MODER14:2;
		uint32_t MODER15:2;
	};
} MODER_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t ODR0:1;
		uint32_t ODR1:1;
		uint32_t ODR2:1;
		uint32_t ODR3:1;
		uint32_t ODR4:1;
		uint32_t ODR5:1;
		uint32_t ODR6:1;
		uint32_t ODR7:1;
		uint32_t ODR8:1;
		uint32_t ODR9:1;
		uint32_t ODR10:1;
		uint32_t ODR11:1;
		uint32_t ODR12:1;
		uint32_t ODR13:1;
		uint32_t ODR14:1;
		uint32_t ODR15:1;
		uint32_t REVERSED:16;
	};
} ODR_reg;


typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t IDR0:1;
		uint32_t IDR1:1;
		uint32_t IDR2:1;
		uint32_t IDR3:1;
		uint32_t IDR4:1;
		uint32_t IDR5:1;
		uint32_t IDR6:1;
		uint32_t IDR7:1;
		uint32_t IDR8:1;
		uint32_t IDR9:1;
		uint32_t IDR10:1;
		uint32_t IDR11:1;
		uint32_t IDR12:1;
		uint32_t IDR13:1;
		uint32_t IDR14:1;
		uint32_t IDR15:1;
		uint32_t REVERSED:16;
	};
} IDR_reg;

// Register map
typedef volatile struct
{
	MODER_reg MODER;
	reg32(OTYPER);
	reg32(OSPEEDR);
	reg32(PUPDR);
	IDR_reg IDR;
	ODR_reg ODR;
	reg32(BSRR);
	reg32(LCKR);
	reg32(AFRL);
	reg32(AFRH);
} gpio_regs;

/****************************************************************************
*									STM32 RCC								*
*									Register Clock Control 					*
*****************************************************************************/
// Field map
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t IOPAEN	: 	1;
		uint32_t IOPBEN	: 	1;
		uint32_t IOPCEN	: 	1;
		uint32_t IOPDEN	: 	1;
		uint32_t IOPEEN	: 	1;
		uint32_t RSVD	: 	2;
		uint32_t IOPHEN	: 	1;
		uint32_t RSVD1	: 	24;
	};
} RCC_IOPENR_reg;

// Register map
typedef volatile struct
{
	reg32(RCC_CR);					// 0x00
	reg32(RCC_ICSCR);				// 0x04
	reg32(RVSD);				    // 0x08
	reg32(RCC_CFGR); 				// 0x0C
	reg32(RCC_CIER); 			    // 0x10
	reg32(RCC_CIFR);			    // 0x14
	reg32(RCC_CICR);				// 0x18
	reg32(RCC_IOPRSTR);				// 0x1C
	reg32(RCC_AHBRSTR);			    // 0x20
	reg32(RCC_APB2RSTR);			// 0x24
	reg32(RCC_APB1RSTR);			// 0x28
	RCC_IOPENR_reg RCC_IOPENR;	    // 0x2C
	reg32(RCC_AHBENR);	            // 0x30
	reg32(RCC_APB2ENR);				// 0x34
	reg32(RCC_APB1ENR);				// 0x38
	reg32(RCC_IOPSMEN);				// 0x3C
	reg32(RCC_AHBSMENR);			// 0x40
	reg32(RCC_APB2SMENR);			// 0x44
	reg32(RCC_APB1SMENR);			// 0x48
	reg32(RCC_CCIPR);				// 0x4C
	reg32(RCC_CSR);			        // 0x50
} rcc_regs;


/****************************************************************************
*									STM32 SYSCFG							*
*									System Configuration Controller			*
*****************************************************************************/
// Field map
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t EXTI0 		: 	4;
		uint32_t EXTI1 		: 	4;
		uint32_t EXTI2 		: 	4;
		uint32_t EXTI3 		: 	4;
		uint32_t RVSD 		: 	16;
	};
} SYSCFG_EXTICR1_reg;

typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t EXTI4 		: 	4;
		uint32_t EXTI5 		: 	4;
		uint32_t EXTI6 		: 	4;
		uint32_t EXTI7 		: 	4;
		uint32_t RVSD 		: 	16;
	};
} SYSCFG_EXTICR2_reg;

typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t EXTI8 		: 	4;
		uint32_t EXTI9 		: 	4;
		uint32_t EXTI10 	: 	4;
		uint32_t EXTI11 	: 	4;
		uint32_t RVSD 		: 	16;
	};
} SYSCFG_EXTICR3_reg;

typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t EXTI12 	: 	4;
		uint32_t EXTI13 	: 	4;
		uint32_t EXTI14 	: 	4;
		uint32_t EXTI15 	: 	4;
		uint32_t RVSD 		: 	16;
	};
} SYSCFG_EXTICR4_reg;


// Register map
typedef struct
{
	reg32(SYSCFG_CFGR1);						// 0x00
	reg32(SYSCFG_CFGR2);						// 0x04
	SYSCFG_EXTICR1_reg SYSCFG_EXTICR1;			// 0x08
	SYSCFG_EXTICR2_reg SYSCFG_EXTICR2;			// 0x0C
	SYSCFG_EXTICR3_reg SYSCFG_EXTICR3;			// 0x10
	SYSCFG_EXTICR4_reg SYSCFG_EXTICR4;			// 0x14
	reg32(RVSD);								// 0x18
	reg32(SYSCFG_CFGR3);						// 0x20
} syscfg_regs;

/****************************************************************************
*									STM32 EXTI								*
*									External Interrupt 						*
*****************************************************************************/
// Field map
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t MR0 	: 	1;
		uint32_t MR1 	: 	1;
		uint32_t MR2 	: 	1;
		uint32_t MR3 	: 	1;
		uint32_t MR4 	: 	1;
		uint32_t MR5 	: 	1;
		uint32_t MR6 	: 	1;
		uint32_t MR7 	: 	1;
		uint32_t MR8 	: 	1;
		uint32_t MR9 	: 	1;
		uint32_t MR10 	: 	1;
		uint32_t MR11 	: 	1;
		uint32_t MR12 	: 	1;
		uint32_t MR13 	: 	1;
		uint32_t MR14 	: 	1;
		uint32_t MR15 	: 	1;
		uint32_t MR16 	: 	1;
		uint32_t MR17 	: 	1;
		uint32_t MR18 	: 	1;
		uint32_t RVSD 	: 	2;
		uint32_t MR21 	: 	1;
		uint32_t MR22 	: 	1;
		uint32_t RVSD1 	: 	9;
	};
} EXTI_IMR_reg;

typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t TR0 	: 	1;
		uint32_t TR1 	: 	1;
		uint32_t TR2 	: 	1;
		uint32_t TR3 	: 	1;
		uint32_t TR4 	: 	1;
		uint32_t TR5 	: 	1;
		uint32_t TR6 	: 	1;
		uint32_t TR7 	: 	1;
		uint32_t TR8 	: 	1;
		uint32_t TR9 	: 	1;
		uint32_t TR10 	: 	1;
		uint32_t TR11 	: 	1;
		uint32_t TR12 	: 	1;
		uint32_t TR13 	: 	1;
		uint32_t TR14 	: 	1;
		uint32_t TR15 	: 	1;
		uint32_t TR16 	: 	1;
		uint32_t TR17 	: 	1;
		uint32_t TR18 	: 	1;
		uint32_t RVSD 	: 	2;
		uint32_t TR21 	: 	1;
		uint32_t TR22 	: 	1;
		uint32_t RVSD1 	: 	9;
	};
} EXTI_TSR_reg;


typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t PR0 	: 	1;
		uint32_t PR1 	: 	1;
		uint32_t PR2 	: 	1;
		uint32_t PR3 	: 	1;
		uint32_t PR4 	: 	1;
		uint32_t PR5 	: 	1;
		uint32_t PR6 	: 	1;
		uint32_t PR7 	: 	1;
		uint32_t PR8 	: 	1;
		uint32_t PR9 	: 	1;
		uint32_t PR10 	: 	1;
		uint32_t PR11 	: 	1;
		uint32_t PR12 	: 	1;
		uint32_t PR13 	: 	1;
		uint32_t PR14 	: 	1;
		uint32_t PR15 	: 	1;
		uint32_t PR16 	: 	1;
		uint32_t PR17 	: 	1;
		uint32_t PR18 	: 	1;
		uint32_t RVSD 	: 	2;
		uint32_t PR21 	: 	1;
		uint32_t PR22 	: 	1;
		uint32_t RVSD1 	: 	9;
	};
} EXTI_PR_reg;

// Register map
typedef struct
{
	EXTI_IMR_reg EXTI_IMR;			// 0x00
	reg32(EXTI_EMR); 				// 0x04
	EXTI_TSR_reg EXTI_RTSR; 		// 0x08
	EXTI_TSR_reg EXTI_FTSR;			// 0x0C
	reg32(EXTI_SWIER); 				// 0x10
	EXTI_PR_reg EXTI_PR;			// 0x14
} ext_regs;


/****************************************************************************
*									STM32 USART								*
*	       Universal synchronous asynchronous receiver trasmitter 			*
*****************************************************************************/
// Field map

// Status register
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t PE 	: 	1;  	// 0
		uint32_t FE 	: 	1;		// 1
		uint32_t NF 	: 	1;		// 2
		uint32_t ORE 	: 	1;		// 3
		uint32_t IDLE 	: 	1;		// 4
		uint32_t RXNE 	: 	1;		// 5
		uint32_t TC 	: 	1;		// 6
		uint32_t TXE 	: 	1;		// 7
		uint32_t LBD 	: 	1;		// 8
		uint32_t CTS 	: 	1;		// 9
		uint32_t RVSD 	: 	22;		// 31:10
	};
} USART_SR_reg;

// Baud Rate register
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t DIV_FRACTION 	: 	4;  // 3:0
		uint32_t DIV_MANTISSA 	: 	12;	// 15:4
	};
} USART_BRR_reg;

// Control Register1
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t SBK 	: 	1;  	// 0
		uint32_t RWU 	: 	1;		// 1
		uint32_t RE 	: 	1;		// 2
		uint32_t TE 	: 	1;		// 3
		uint32_t IDLEIE : 	1;		// 4
		uint32_t RXNEIE	: 	1;		// 5
		uint32_t TCIE 	: 	1;		// 6
		uint32_t TXEIE 	: 	1;		// 7
		uint32_t PEIE 	: 	1;		// 8
		uint32_t PS 	: 	1;		// 9
		uint32_t PCE 	: 	1;		// 10
		uint32_t WAKE 	: 	1;		// 11
		uint32_t M 		: 	1;		// 12
		uint32_t UE 	: 	1;		// 13
		uint32_t RVSD 	: 	1;		// 14
		uint32_t OVERS 	: 	1;		// 15
		uint32_t RVSD1 	: 	16;		// 31:16
	};
} USART_CR1_reg;

// Control Register2
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t ADD 	: 	4;  	// 3:0
		uint32_t RVSD 	: 	1;		// 4
		uint32_t LBDL 	: 	1;		// 5
		uint32_t LBDIE 	: 	1;		// 6
		uint32_t RVSD1  : 	1;		// 7
		uint32_t LBCL	: 	1;		// 8
		uint32_t CPHA 	: 	1;		// 9
		uint32_t CPOL 	: 	1;		// 10
		uint32_t CLKEN 	: 	1;		// 11
		uint32_t STOP 	: 	2;		// 13:12
		uint32_t LINEN 	: 	1;		// 14
		uint32_t RVSD2 	: 	1;		// 31:15
	};
} USART_CR2_reg;

// Control Register3
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t EIE 	: 	1;  	// 0
		uint32_t IREN 	: 	1;		// 1
		uint32_t IRLP 	: 	1;		// 2
		uint32_t HDSEL 	: 	1;		// 3
		uint32_t NACK : 	1;		// 4
		uint32_t SCEN	: 	1;		// 5
		uint32_t DMAR 	: 	1;		// 6
		uint32_t DMAT 	: 	1;		// 7
		uint32_t RTSE 	: 	1;		// 8
		uint32_t CTSE 	: 	1;		// 9
		uint32_t CTSIE 	: 	1;		// 10
		uint32_t ONEBIT	: 	1;		// 11
		uint32_t RVSD 	: 	20;		// 31:12
	};
} USART_CR3_reg;

// Guard time and prescaler register
typedef union
{
	uint32_t Register;
	struct
	{
		uint32_t PSC	:	8;		// 7:0
		uint32_t GT		:	8;		// 15:8
		uint32_t RVSD 	:	16;		// 31:6
	};
} USART_GTPR_reg;

// Register map
typedef struct
{
	USART_SR_reg USART_SR;			// 0x00
	reg32(USART_DR);				// 0x04
	USART_BRR_reg USART_BRR;		// 0x08
	USART_CR1_reg USART_CR1;		// 0x0C
	USART_CR2_reg USART_CR2;		// 0x10
	USART_CR3_reg USART_CR3;		// 0x14
	USART_GTPR_reg USART_GTPR;		// 0x18
} usart_regs;



#define GPIOA 		((gpio_regs*) 			GPIOA_BASE)
#define GPIOB 		((gpio_regs*) 			GPIOB_BASE)
#define GPIOC 		((gpio_regs*) 			GPIOC_BASE)
#define GPIOD 		((gpio_regs*) 			GPIOD_BASE)
#define GPIOE 		((gpio_regs*) 			GPIOE_BASE)
#define GPIOH 		((gpio_regs*) 			GPIOH_BASE)
#define RCC 		((rcc_regs*) 			RCC_BASE)
#define SYSCFG 	((syscfg_regs*) 		SYSCFG_BASE)
#define EXTI 		((ext_regs*) 			EXTI_BASE)
#define USART1 	((usart_regs*)			USART1_BASE)
#define USART2 	((usart_regs*)			USART2_BASE)
#define USART6 	((usart_regs*)			USART6_BASE)

#define NVIC_ISER0 ((uint32_t*)			NVIC_ISER0_BASE)
#define NVIC_ISER1 ((uint32_t*)			NVIC_ISER1_BASE)
#define NVIC_ISER2 ((uint32_t*)			NVIC_ISER2_BASE)
#define NVIC_ISER3 ((uint32_t*)			NVIC_ISER3_BASE)

#endif //MCUSTM32F411E_H
