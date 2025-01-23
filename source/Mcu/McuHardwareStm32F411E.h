//
// Created by Phan Linh on 25/12/2024.
//

#ifndef MCUSTM32F411E_H
#define MCUSTM32F411E_H

#define FPU 							16000000

#define PERIPH_BASE           			0x40000000U
#define AHB1PERIPH_BASE       			(PERIPH_BASE + 0x00020000U)
#define RCC_BASE              			(AHB1PERIPH_BASE 	+ 0x3800U)
#define GPIOA_BASE            			(AHB1PERIPH_BASE 	+ 0x0000U)
#define GPIOB_BASE            			(AHB1PERIPH_BASE 	+ 0x0400U)
#define GPIOC_BASE            			(AHB1PERIPH_BASE 	+ 0x0800U)
#define GPIOD_BASE            			(AHB1PERIPH_BASE 	+ 0x0C00U)
#define GPIOE_BASE            			(AHB1PERIPH_BASE 	+ 0x1000U)
#define GPIOH_BASE            			(AHB1PERIPH_BASE 	+ 0x1C00U)
#define EXTI_BASE 		  				(PERIPH_BASE 		+ 0x13C00U)
#define SYSCFG_BASE 		  			(PERIPH_BASE 		+ 0x13800U)
#define SPI4_BASE 						0x40013400U
#define SPI1_BASE 						0x40013000U
#define SDI0_BASE 						0x40012C00U
#define ADC1_BASE 						0x40012000U
#define USART6_BASE						0x40011400U
#define USART1_BASE 					0x40011000U
#define TIM1_BASE 						0x40010000U
#define PWR_BASE 						0x40007000U
#define I2C3_BASE 						0x40005C00U
#define I2C2_BASE 						0x40005800U
#define I2C1_BASE 						0x40005400U
#define USART2_BASE 					0x40004400U




// ARM Address
#define NVIC_ISER0_BASE					0xE000E100U
#define NVIC_ISER1_BASE					0xE000E104U
#define NVIC_ISER2_BASE					0xE000E108U
#define NVIC_ISER3_BASE					0xE000E10CU
#define NVIC_ISER4_BASE					0xE000E110U
#define NVIC_ISER5_BASE					0xE000E114U
#define NVIC_ISER6_BASE					0xE000E118U
#define NVIC_ISER7_BASE					0xE000E11CU

#define SCS_BASE            			0xE000E000U       			// System Control Space Base Address
#define SCB_BASE            			(SCS_BASE +  0x0D00)        // System Control Block Base Address

#define SYSTICK_BASE 					0xE000E010U


#define reg16( r ) unsigned short r
#define reg32( r ) unsigned long r


/****************************************************************************
*									STM32 GPIOA_BASE						*
*									General Purpose I/O						*
*****************************************************************************/
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
		uint32_t PUPDR0:2;
		uint32_t PUPDR1:2;
		uint32_t PUPDR2:2;
		uint32_t PUPDR3:2;
		uint32_t PUPDR4:2;
		uint32_t PUPDR5:2;
		uint32_t PUPDR6:2;
		uint32_t PUPDR7:2;
		uint32_t PUPDR8:2;
		uint32_t PUPDR9:2;
		uint32_t PUPDR10:2;
		uint32_t PUPDR11:2;
		uint32_t PUPDR12:2;
		uint32_t PUPDR13:2;
		uint32_t PUPDR14:2;
		uint32_t PUPDR15:2;
	};
} PUPDR_reg;

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

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t AFRL0:4; 		// 3:0
		uint32_t AFRL1:4;		// 7:4
		uint32_t AFRL2:4;		// 11:8
		uint32_t AFRL3:4;		// 15:12
		uint32_t AFRL4:4;		// 19:16
		uint32_t AFRL5:4;		// 23:20
		uint32_t AFRL6:4;		// 27:24
		uint32_t AFRL7:4;		// 31:28
	};
} AFRL_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t AFRH0:4;
		uint32_t AFRH1:4;
		uint32_t AFRH2:4;
		uint32_t AFRH3:4;
		uint32_t AFRH4:4;
		uint32_t AFRH5:4;
		uint32_t AFRH6:4;
		uint32_t AFRH7:4;
	};
} AFRH_reg;

// Register map
typedef volatile struct
{
	MODER_reg MODER;
	reg32(OTYPER);
	reg32(OSPEEDR);
	PUPDR_reg PUPDR;
	IDR_reg IDR;
	ODR_reg ODR;
	reg32(BSRR);
	reg32(LCKR);
	AFRL_reg AFRL;
	AFRH_reg AFRH;
	// reg32(AFR[2]);
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
		uint32_t SW			:	2; 		// 1:0
		uint32_t SWS		: 	2;		// 3:2
		uint32_t HPRE		: 	4;		// 7:4
		uint32_t RVSD		:	2;		// 9:8
		uint32_t PPRE1		: 	3;		// 12:10
		uint32_t PPRE2		: 	3;		// 15:13
		uint32_t RTCPRE		: 	5;		// 20:16
		uint32_t MC01		: 	2;		// 22:21
		uint32_t I2SSRC		: 	1;		// 23
		uint32_t MC01PRE	: 	3;		// 26:24
		uint32_t MC02PRE	: 	3;		// 29:27
		uint32_t MC02		: 	2;		// 31:30
	};
} RCC_CFGR_reg;


typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t GPIOAEN	: 	1;
		uint32_t GPIOBEN	: 	1;
		uint32_t GPIOCEN	: 	1;
		uint32_t GPIODEN	: 	1;
		uint32_t GPIOEEN	: 	1;
		uint32_t RSVD		: 	2;
		uint32_t GPIOHEN	: 	1;
		uint32_t RSVD1		: 	4;
		uint32_t CRCEN		: 	1;
		uint32_t RSVD2		: 	8;
		uint32_t DMA1EN		: 	1;
		uint32_t DMA2EN		: 	1;
		uint32_t RSVD3		: 	9;
	};
} RCC_AHB1ENR_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t TIM2EN		: 	1; 	// 0
		uint32_t TIM3EN		: 	1;	// 1
		uint32_t TIM4EN		: 	1;	// 2
		uint32_t TIM5EN		: 	1;	// 3
		uint32_t RSVD		: 	7;	// 10:4
		uint32_t WWDGEN		: 	1;	// 11
		uint32_t RSVD1		: 	2;	// 13:12
		uint32_t SPI2EN		: 	1;	// 14
		uint32_t SPI3EN		: 	1;	// 15
		uint32_t RSVD2		: 	1;	// 16
		uint32_t USART2EN	: 	1;	// 17
		uint32_t RSVD3		: 	3;	// 20:18
		uint32_t I2C1EN		: 	1;	// 21
		uint32_t I2C2EN		: 	1;	// 22
		uint32_t I2C3EN		: 	1;	// 23
		uint32_t RSVD4		: 	4;	// 27:24
		uint32_t PWREN		: 	1;	// 28
		uint32_t RVSD5		: 	3; 	// 31:29
	};	
} RCC_APB1ENR_reg;


typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t TIM1EN		: 	1; 	// 0
		uint32_t RSVD		: 	3;	// 3:1
		uint32_t USART1EN	: 	1;	// 4
		uint32_t USART6EN	: 	1;	// 5		
		uint32_t RSVD1		: 	2;	// 7:6
		uint32_t ADC1EN		: 	1;	// 8
		uint32_t RSVD2		: 	2;	// 10:9
		uint32_t SDIOEN		: 	1;	// 11
		uint32_t SPI1EN		: 	1;	// 12
		uint32_t SPI4EN		: 	1;	// 13
		uint32_t SYSCFGEN	: 	1;	// 14
		uint32_t RVSD3		: 	1;	// 15
		uint32_t TIM9EN		: 	1;	// 16
		uint32_t TIM10EN	: 	1;	// 17
		uint32_t TIM11EN	: 	1;	// 18
		uint32_t RVSD4		: 	1;	// 19
		uint32_t SPIEN5		: 	1; 	// 20
		uint32_t RVSD5		: 	11;	// 31:21
	};	
} RCC_APB2ENR_reg;


// Register map
typedef volatile struct
{
	reg32(RCC_CR);					// 0x00
	reg32(RCC_PLLCFGR);				// 0x04
	RCC_CFGR_reg RCC_CFGR;	
	reg32(RCC_CIR); 				// 0x0C
	reg32(RCC_AHB1RSTR); 			// 0x10
	reg32(RCC_AHB2RSTR);			// 0x14
	reg32(RSVD0);					// 0x18
	reg32(RSVD1);					// 0x1C
	reg32(RCC_APB1RSTR);			// 0x20
	reg32(RCC_APB2RSTR);			// 0x24
	reg32(RSVD2);					// 0x28
	reg32(RSVD3);					// 0x2C
	RCC_AHB1ENR_reg RCC_AHB1ENR;	// 0x30
	reg32(RCC_AHB2ENR);				// 0x34
	reg32(RSVD4);					// 0x38
	reg32(RSVD5);					// 0x3C
	RCC_APB1ENR_reg RCC_APB1ENR;	// 0x40
	RCC_APB2ENR_reg RCC_APB2ENR;	// 0x44
	reg32(RSVD6);					// 0x48
	reg32(RSVD7);					// 0x4C
	reg32(RCC_AHB1LPENR);			// 0x50
	reg32(RCC_AHB2LPENR);			// 0x54
	reg32(RSVD8);					// 0x58
	reg32(RSVD9);					// 0x5C
	reg32(RCC_APB1LPENR);			// 0x60
	reg32(RCC_APB2LPENR);			// 0x64
	reg32(RSVD10);					// 0x68
	reg32(RSVD11);					// 0x6C
	reg32(RCC_BDCR);				// 0x70
	reg32(RCC_CSR);					// 0x74
	reg32(RSVD12);					// 0x78
	reg32(RSVD13);					// 0x7C
	reg32(RCC_SSCGR);				// 0x80
	reg32(RCC_PLLI2SCFG);			// 0x84
} rcc_regs;


/****************************************************************************
*									STM32 SYSCFG							*
*									System Configuration Controller			*
*****************************************************************************/
// Field map
typedef volatile union
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

typedef volatile union
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

typedef volatile union
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

typedef volatile union
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
	reg32(SYSCFG_MEMRMP);						// 0x00
	reg32(SYSCFG_PMC);							// 0x04
	SYSCFG_EXTICR1_reg SYSCFG_EXTICR1;			// 0x08
	SYSCFG_EXTICR2_reg SYSCFG_EXTICR2;			// 0x0C
	SYSCFG_EXTICR3_reg SYSCFG_EXTICR3;			// 0x10
	SYSCFG_EXTICR4_reg SYSCFG_EXTICR4;			// 0x14
	reg32(RVSD);								// 0x18
	reg32(SYSCFG_CMPCR);						// 0x20
} syscfg_regs;

/****************************************************************************
*									STM32 EXTI								*
*									External Interrupt 						*
*****************************************************************************/
// Field map
typedef volatile union
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

typedef volatile union
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


typedef volatile union
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
// Status register
typedef volatile union
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
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t DIV_FRACTION 	: 	4;  // 3:0
		uint32_t DIV_MANTISSA 	: 	12;	// 15:4
	};
} USART_BRR_reg;

// Control Register1
typedef volatile union
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
		uint32_t OVER8 	: 	1;		// 15
		uint32_t RVSD1 	: 	16;		// 31:16
	};
} USART_CR1_reg;

// Control Register2
typedef volatile union
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
typedef volatile union
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
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t PSC	:	8;		// 7:0
		uint32_t GT		:	8;		// 15:8
		uint32_t RVSD 	:	16;		// 31:6
	};
} USART_GTPR_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t DR		:	9;		// 8:0
		uint32_t RVSD 	:	23;		// 31:6
	};
} USART_DR_reg;

// Register map
typedef struct
{
	USART_SR_reg USART_SR;			// 0x00
	USART_DR_reg USART_DR;			// 0x04
	USART_BRR_reg USART_BRR;		// 0x08
	USART_CR1_reg USART_CR1;		// 0x0C
	USART_CR2_reg USART_CR2;		// 0x10
	USART_CR3_reg USART_CR3;		// 0x14
	USART_GTPR_reg USART_GTPR;		// 0x18
} usart_regs;

/****************************************************************************
*									SYSTICK								    *
*									Arm Cortex M4 						    *
*****************************************************************************/
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t ENABLE			:	1;      // 0
		uint32_t TICKINT		:	1;      // 1
		uint32_t CLKSOURCE		:	1;      // 2
		uint32_t RVSD		    :	13;     // 15:3
		uint32_t COUNTFLAG		:	1;      // 16
		uint32_t RVSD1		    :	15;     // 31:17
	};
} STK_CTRL_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t RELOAD		    :	24;     // 23:0
		uint32_t RVSD		    :	8;      // 31:24
	};
} STK_LOAD_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t CURRENT		:	24;     // 23:0
		uint32_t RVSD		    :	8;      // 31:24
	};
} STK_VAL_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t TENMS		    :	24;     // 23:0
		uint32_t RVSD		    :	6;      // 29:24
	};
} STK_CALIB_reg;

// Register map
typedef struct
{
	STK_CTRL_reg STK_CTRL; 				    // 0x00
	STK_LOAD_reg STK_LOAD; 				    // 0x04
	STK_VAL_reg STK_VAL;					// 0x08
	STK_CALIB_reg STK_CALIB;				// 0x0C
} systick_regs;

/********************************************************************************
 * 									STM32 I2C									*
 * 						Inter-integrated circuit Interface						*
 * ******************************************************************************/
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t PE		    	:	1;     // 0
		uint32_t SMBUS	    	:	1;     // 1
		uint32_t RVSD	    	:	1;     // 2
		uint32_t SMBTYPE    	:	1;     // 3
		uint32_t ENARP	    	:	1;     // 4
		uint32_t ENPEC	    	:	1;     // 5
		uint32_t ENGC	    	:	1;     // 6
		uint32_t NOSTRETCH    	:	1;     // 7
		uint32_t START	    	:	1;     // 8
		uint32_t STOP	    	:	1;     // 9
		uint32_t ACK	    	:	1;     // 10
		uint32_t POS	    	:	1;     // 11
		uint32_t PEC	    	:	1;     // 12	
		uint32_t ALERT	    	:	1;     // 13
		uint32_t RVSD1	    	:	1;     // 14
		uint32_t SWRST	    	:	1;     // 15
		uint32_t RVSD2	    	:	16;     // 31:16	
	};
} I2C_CR1_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t FREQ	    	:	6;     // 5:0
		uint32_t RVSD	    	:	2;     // 7:6
		uint32_t ITERREN	    :	1;     // 8
		uint32_t ITEVTEN    	:	1;     // 9
		uint32_t ITBUFEN    	:	1;     // 10
		uint32_t DMAEN	    	:	1;     // 11
		uint32_t LAST	    	:	1;     // 12		
		uint32_t RVSD1	    	:	19;    // 31:13
	};
} I2C_CR2_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t ADD0	    	:	1;     // 0
		uint32_t ADD71	    	:	7;     // 7:1
		uint32_t ADD98	    	:	2;     // 9:8
		uint32_t RVSD    		:	5;     // 14:10
		uint32_t ADDMODE    	:	1;     // 15	
		uint32_t RVSD1	    	:	16;    // 31:16
	};
} I2C_OAR1_reg;


typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t ENDUAL	    	:	1;     // 0
		uint32_t ADD2	    	:	7;     // 7:1
		uint32_t RVSD	    	:	24;    // 31:8
	};
} I2C_OAR2_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t DR	    		:	8;     // 7:0
		uint32_t RVSD	    	:	24;    // 31:8
	};
} I2C_DR_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t SB		    	:	1;     // 0
		uint32_t ADDR	    	:	1;     // 1
		uint32_t BTF	    	:	1;     // 2
		uint32_t ADD10    		:	1;     // 3
		uint32_t STOPF	    	:	1;     // 4
		uint32_t RVSD	    	:	1;     // 5
		uint32_t RXNE	    	:	1;     // 6
		uint32_t TXE    		:	1;     // 7
		uint32_t BERR	    	:	1;     // 8
		uint32_t ARLO	    	:	1;     // 9
		uint32_t AF	    		:	1;     // 10
		uint32_t OVR	    	:	1;     // 11
		uint32_t PECERR	    	:	1;     // 12	
		uint32_t RVSD1	    	:	1;     // 13
		uint32_t TIMEOUT	    :	1;     // 14
		uint32_t SMBALERT	    :	1;     // 15
		uint32_t RVSD2	    	:	16;    // 31:16	
	};
} I2C_SR1_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t MSL		    :	1;     // 0
		uint32_t BUSY	    	:	1;     // 1
		uint32_t TRA	    	:	1;     // 2
		uint32_t RVSD    		:	1;     // 3
		uint32_t GENCALL	    :	1;     // 4
		uint32_t SMBDEFAULT	    :	1;     // 5
		uint32_t SMBHOST	    :	1;     // 6
		uint32_t DUALF    		:	1;     // 7
		uint32_t PEC	    	:	8;     // 15:8
		uint32_t RVSD2	    	:	16;    // 31:16	
	};
} I2C_SR2_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t CCR		    :	12;    // 11:0
		uint32_t RVSD	    	:	2;     // 13:12
		uint32_t DUTY	    	:	1;     // 14
		uint32_t FS    			:	1;     // 15
		uint32_t RVSD2	    	:	16;    // 31:16	
	};
} I2C_CCR_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t TRISE	    	:	6;     // 5:0
		uint32_t RVSD	    	:	26;    // 31:6
	};
} I2C_TRISE_reg;

typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t DNF	    	:	4;     	// 3:0
		uint32_t ANOFF	    	:	1;    	// 4
		uint32_t RVSD 			:	27;		// 31:5
	};
} I2C_FLTR_reg;


 // Register map
typedef struct
{
	I2C_CR1_reg I2C_CR1; 				    	// 0x00
	I2C_CR2_reg I2C_CR2; 				    	// 0x04
	I2C_OAR1_reg I2C_OAR1; 				    	// 0x08
	I2C_OAR2_reg I2C_OAR2; 				    	// 0x0C
	I2C_DR_reg I2C_DR; 				    		// 0x10
	I2C_SR1_reg I2C_SR1; 				    	// 0x14
	I2C_SR2_reg I2C_SR2; 				    	// 0x18
	I2C_CCR_reg I2C_CCR; 				    	// 0x1C
	I2C_TRISE_reg I2C_TRISE; 				    // 0x20
	I2C_FLTR_reg I2C_FLTR; 				    	// 0x24
} i2c_regs;


/****************************************************************************
*								SYSTEM CONTROL BLOCK				    	*
*									Arm Cortex M4 						    *
*****************************************************************************/
typedef volatile union
{
	uint32_t Register;
	struct
	{
		uint32_t VECTRESET		:	1;     	// 0
		uint32_t VECTCLRACTIVE  :	1;      // 1
		uint32_t SYSRESETREQ  	:	1;		// 2
		uint32_t RVSD			: 	5;		// 7:3
		uint32_t PRIGROUP		: 	3; 		// 10:8
		uint32_t RVSD1			:	4;		// 14:11
		uint32_t ENDIANESS		:	1;		// 15
		uint32_t VECTKEY		: 	16;		// 31:16
	};
} AIR_reg;

typedef struct
{
	reg32(PUID);                        	// CPU ID Base Register                                     
	reg32(ICSR);                         	// Interrupt Control State Register                        
	reg32(VTOR);                         	// Vector Table Offset Register                            
	AIR_reg AIRCR;                        	// Application Interrupt / Reset Control Register           
	reg32(SCR);                          	// System Control Register                                  
	reg32(CCR);                          	// Configuration Control Register                           
	reg32(RVSD[3]);                      	// System Handlers Priority Registers (4-7, 8-11, 12-15)    
	reg32(SHCSR);                        	// System Handler Control and State Register                
	reg32(CFSR);                         	// Configurable Fault Status Register                       
	reg32(HFSR);                         	// Hard Fault Status Register                               
	reg32(DFSR);                         	// Debug Fault Status Register                              
	reg32(MMFAR);                        	// Mem Manage Address Register                             
	reg32(BFAR);                         	// Bus Fault Address Register
	reg32(AFSR);                         	// Auxiliary Fault Status Register
	reg32(PFR[2]);                       	// Processor Feature Register
	reg32(DFR);                          	// Debug Feature Register
	reg32(ADR);                          	// Auxiliary Feature Register
	reg32(MMFR[4]);                      	// Memory Model Feature Register
	reg32(ISAR[5]);                      	// ISA Feature Register
} scb_reg;

#define GPIOA 		((gpio_regs*) 			GPIOA_BASE)
#define GPIOB 		((gpio_regs*) 			GPIOB_BASE)
#define GPIOC 		((gpio_regs*) 			GPIOC_BASE)
#define GPIOD 		((gpio_regs*) 			GPIOD_BASE)
#define GPIOE 		((gpio_regs*) 			GPIOE_BASE)
#define GPIOH 		((gpio_regs*) 			GPIOH_BASE)
#define RCC 		((rcc_regs*) 			RCC_BASE)
#define SYSCFG 		((syscfg_regs*) 		SYSCFG_BASE)
#define EXTI 		((ext_regs*) 			EXTI_BASE)
#define USART1 		((usart_regs*)			USART1_BASE)
#define USART2 		((usart_regs*)			USART2_BASE)
#define USART6 		((usart_regs*)			USART6_BASE)

#define NVIC_ISER0 	((uint32_t*)			NVIC_ISER0_BASE)
#define NVIC_ISER1 	((uint32_t*)			NVIC_ISER1_BASE)
#define NVIC_ISER2 	((uint32_t*)			NVIC_ISER2_BASE)
#define NVIC_ISER3 	((uint32_t*)			NVIC_ISER3_BASE)

#define SYSTICK 	((systick_regs*) 		SYSTICK_BASE)

#define I2C1 		((i2c_regs*)			I2C1_BASE)
#define I2C2 		((i2c_regs*)			I2C2_BASE)
#define I2C3 		((i2c_regs*)			I2C3_BASE)

#define SCB         ((scb_reg *)           	SCB_BASE)
#define NVIC_AIRCR_VECTKEY    (0x5FA << 16)   // AIRCR Key for write access   */
#define NVIC_SYSRESETREQ            2         // System Reset Request         */


#endif //MCUSTM32F411E_H
