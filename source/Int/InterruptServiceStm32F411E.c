//
// Created by Phan Linh on 25/12/2024.
//

#include "InterruptHardwareStm32F411E.h"

// #Delay
void DelayUs( uint32_t u32_TimeInUs )
{
	for (uint32_t volatile i = 0; i < ( u32_TimeInUs * FPU / 7 ); i++);
}

void DelayMs( uint32_t u32_TimeInMs )
{
	for (uint32_t volatile i = 0; i < ( u32_TimeInMs * 1000 * FPU / 7  ); i++);
}

void Delay( uint32_t u32_DelayLoop )
{
	for (uint32_t i = 0; i < u32_DelayLoop; i++);
}


/****************************************************************************
*									STM32 EXTI								*
*									External Interrupt 						*
*****************************************************************************/
static void inline is_EnableExtiInterrupt()
{
	// APB2 SYSCFG Clock Enable
	RCC->RCC_APB2ENR |= 1 << 14;

}

static void inline is_SetExtiInterrupt(char* pinString)
{
	char port; int exti;
	sscanf(pinString, "%c%d", &port,&exti);
	// Set EXTI line for port number
	switch (exti)
	{
		case 0:
			SYSCFG->SYSCFG_EXTICR1.EXTI0 = (port - 'A'); 	//  EXTI0
			*NVIC_ISER0 |= 1 << 6;
			break;
		case 1:
			SYSCFG->SYSCFG_EXTICR1.EXTI1 = (port - 'A'); 	//  EXTI1
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 2:
			SYSCFG->SYSCFG_EXTICR1.EXTI2 = (port - 'A'); 	//  EXTI2
			*NVIC_ISER0 |= 1 << 8;
			break;
		case 3:
			SYSCFG->SYSCFG_EXTICR1.EXTI3 = (port - 'A'); 	//  EXTI3
			*NVIC_ISER0 |= 1 << 9;
			break;			
		case 4:
			SYSCFG->SYSCFG_EXTICR2.EXTI4 = (port - 'A'); 	//  EXTI4
			*NVIC_ISER0 |= 1 << 10;
			break;
		case 5:
			SYSCFG->SYSCFG_EXTICR2.EXTI5 = (port - 'A'); 	//  EXTI5
			*NVIC_ISER0 |= 1 << 23;
			break;
		case 6:
			SYSCFG->SYSCFG_EXTICR2.EXTI6 = (port - 'A'); 	//  EXTI6
			*NVIC_ISER0 |= 1 << 23;
			break;			
		case 7:
			SYSCFG->SYSCFG_EXTICR2.EXTI7 = (port - 'A'); 	//  EXTI7
			*NVIC_ISER0 |= 1 << 23;
			break;
		case 8:
			SYSCFG->SYSCFG_EXTICR3.EXTI8 = (port - 'A'); 	//  EXTI8
			*NVIC_ISER0 |= 1 << 23;
			break;
		case 9:
			SYSCFG->SYSCFG_EXTICR3.EXTI9 = (port - 'A'); 	//  EXTI9
			*NVIC_ISER0 |= 1 << 23;
			break;
		case 10:
			SYSCFG->SYSCFG_EXTICR3.EXTI10 = (port - 'A'); 	//  EXTI10
			*NVIC_ISER1 |= 1 << 8;
			break;
		case 11:
			SYSCFG->SYSCFG_EXTICR3.EXTI11 = (port - 'A'); 	//  EXTI11
			*NVIC_ISER1 |= 1 << 8;
			break;
		case 12:
			SYSCFG->SYSCFG_EXTICR4.EXTI12 = (port - 'A'); 	//  EXTI12
			*NVIC_ISER1 |= 1 << 8;
			break;			
		case 13:
			SYSCFG->SYSCFG_EXTICR4.EXTI13 = (port - 'A'); 	//  EXTI13
			*NVIC_ISER1 |= 1 << 8;
			break;			
		case 14:
			SYSCFG->SYSCFG_EXTICR4.EXTI14 = (port - 'A'); 	//  EXTI14
			*NVIC_ISER1 |= 1 << 8;
			break;
		case 15:
			SYSCFG->SYSCFG_EXTICR4.EXTI15 = (port - 'A'); 	//  EXTI15
			*NVIC_ISER1 |= 1 << 8;
			break;
	} 
	// Disable EXTI0 Interrupt Masking
	EXTI->EXTI_IMR.Register |= ( 1 << exti );
	// Set rising edge detection
	EXTI->EXTI_RTSR.Register |= ( 1 << exti );
}

static void inline is_ClearExtiPendingInterrupt(char* pinString)
{
    char port; int exti;
	sscanf(pinString, "%c%d", &port,&exti);
    EXTI->EXTI_PR.Register |= ( 1 << exti );
}


void InitIsr(char* pinString, void (*pfServiceFunction)(void))
{
    char port; int exti;
    sscanf(pinString, "%c%d", &port,&exti);
    switch (exti)
    {
        case 0:
            pfServiceExti0Irq = pfServiceFunction;
            break;
        case 1:
            pfServiceExti1Irq = pfServiceFunction;
            break;
        case 2:
            pfServiceExti2Irq = pfServiceFunction;
            break;
        case 3:
            pfServiceExti3Irq = pfServiceFunction;
            break;
        case 4:
            pfServiceExti4Irq = pfServiceFunction;
            break;
        case 5:
            pfServiceExti5Irq = pfServiceFunction;
            break;
        case 6:
            pfServiceExti6Irq = pfServiceFunction;
            break;
        case 7:
            pfServiceExti7Irq = pfServiceFunction;
            break;
        case 8:
            pfServiceExti8Irq = pfServiceFunction;
            break;
        case 9:
            pfServiceExti9Irq = pfServiceFunction;
            break;
        case 10:
            pfServiceExti10Irq = pfServiceFunction;
            break;
        case 11:
            pfServiceExti11Irq = pfServiceFunction;
            break;
        case 12:
            pfServiceExti12Irq = pfServiceFunction;
            break;
        case 13:
            pfServiceExti13Irq = pfServiceFunction;
            break;
        case 14:
            pfServiceExti14Irq = pfServiceFunction;
            break;
        case 15:
            pfServiceExti15Irq = pfServiceFunction;
            break;
    }

}

void EXTI0_IRQHandler(void)
{
	(*pfServiceExti0Irq)();
}

void EXTI1_IRQHandler(void)
{
	(*pfServiceExti1Irq)();
}

void EXTI2_IRQHandler(void)
{
    (*pfServiceExti2Irq)();
}

void EXTI3_IRQHandler(void)
{
    (*pfServiceExti3Irq)();
}

void EXTI4_IRQHandler(void)
{
    (*pfServiceExti4Irq)();
}

void EXTI5_IRQHandler(void)
{
    (*pfServiceExti5Irq)();
}

void EXTI6_IRQHandler(void)
{
    (*pfServiceExti6Irq)();
}

void EXTI7_IRQHandler(void)
{
    (*pfServiceExti7Irq)();
}

void EXTI8_IRQHandler(void)
{
    (*pfServiceExti8Irq)();
}

void EXTI9_IRQHandler(void)
{
    (*pfServiceExti9Irq)();
}

void EXTI10_IRQHandler(void)
{
    (*pfServiceExti10Irq)();
}

void EXTI11_IRQHandler(void)
{
    (*pfServiceExti11Irq)();
}

void EXTI12_IRQHandler(void)
{
    (*pfServiceExti12Irq)();
}

void EXTI13_IRQHandler(void)
{
    (*pfServiceExti13Irq)();
}

void EXTI14_IRQHandler(void)
{
    (*pfServiceExti14Irq)();
}

void EXTI15_IRQHandler(void)
{
    (*pfServiceExti15Irq)();
}