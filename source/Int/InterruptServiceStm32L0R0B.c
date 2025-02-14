//
// Created by Phan Linh on 25/12/2024.
//

#include "InterruptHardwareStm32L0R0B.h"

/****************************************************************************
*									STM32 EXTI								*
*									External Interrupt 						*
*****************************************************************************/
static void inline is_EnableExtiInterrupt()
{
	// APB2 SYSCFG Clock Enable
	RCC->RCC_APB2ENR |= 1 << 0;
}

static void inline is_SetUsart2Interrupt()
{
	// Position: 28
	*NVIC_ISER0 |=  1 << 28;
	*NVIC_IPR7  &= 0xFFFFFF00;
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
			*NVIC_ISER0 |= 1 << 5;
			break;
		case 1:
			SYSCFG->SYSCFG_EXTICR1.EXTI1 = (port - 'A'); 	//  EXTI1
			*NVIC_ISER0 |= 1 << 5;
			break;
		case 2:
			SYSCFG->SYSCFG_EXTICR1.EXTI2 = (port - 'A'); 	//  EXTI2
			*NVIC_ISER0 |= 1 << 6;
			break;
		case 3:
			SYSCFG->SYSCFG_EXTICR1.EXTI3 = (port - 'A'); 	//  EXTI3
			*NVIC_ISER0 |= 1 << 6;
			break;			
		case 4:
			SYSCFG->SYSCFG_EXTICR2.EXTI4 = (port - 'A'); 	//  EXTI4
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 5:
			SYSCFG->SYSCFG_EXTICR2.EXTI5 = (port - 'A'); 	//  EXTI5
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 6:
			SYSCFG->SYSCFG_EXTICR2.EXTI6 = (port - 'A'); 	//  EXTI6
			*NVIC_ISER0 |= 1 << 7;
			break;			
		case 7:
			SYSCFG->SYSCFG_EXTICR2.EXTI7 = (port - 'A'); 	//  EXTI7
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 8:
			SYSCFG->SYSCFG_EXTICR3.EXTI8 = (port - 'A'); 	//  EXTI8
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 9:
			SYSCFG->SYSCFG_EXTICR3.EXTI9 = (port - 'A'); 	//  EXTI9
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 10:
			SYSCFG->SYSCFG_EXTICR3.EXTI10 = (port - 'A'); 	//  EXTI10
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 11:
			SYSCFG->SYSCFG_EXTICR3.EXTI11 = (port - 'A'); 	//  EXTI11
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 12:
			SYSCFG->SYSCFG_EXTICR4.EXTI12 = (port - 'A'); 	//  EXTI12
			*NVIC_ISER0 |= 1 << 7;
			break;			
		case 13:
			SYSCFG->SYSCFG_EXTICR4.EXTI13 = (port - 'A'); 	//  EXTI13
			*NVIC_ISER0 |= 1 << 7;
			break;			
		case 14:
			SYSCFG->SYSCFG_EXTICR4.EXTI14 = (port - 'A'); 	//  EXTI14
			*NVIC_ISER0 |= 1 << 7;
			break;
		case 15:
			SYSCFG->SYSCFG_EXTICR4.EXTI15 = (port - 'A'); 	//  EXTI15
			*NVIC_ISER0 |= 1 << 7;
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

static void is_InitUsart2Isr(void (*pfServiceFunction)(void))
{
	pfServiceUsart2Irq = pfServiceFunction;
}
void InitIsr(char* pinString, void (*pfServiceFunction) (void))
{
    char port; int exti;
    sscanf(pinString, "%c%d", &port,&exti);
    switch (exti)
    {
        case 0:
        case 1:
            pfServiceExti01Irq = pfServiceFunction;
            break;
        case 2:
        case 3:
            pfServiceExti23Irq = pfServiceFunction;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            pfServiceExti415Irq = pfServiceFunction;
            break;
    }

}

void EXTI0_1_IRQHandler(void)
{
	(*pfServiceExti01Irq)();
}

void EXTI2_3_IRQHandler(void)
{
	(*pfServiceExti23Irq)();
}

void EXTI4_15_IRQHandler(void)
{
    (*pfServiceExti415Irq)();
}

void USART2_IRQHandler(void)
{
	(*pfServiceUsart2Irq)();
}
/********************************************************************************
 * 									ARM SYS TICK								*
 * 								System Tick Timer								*
 * ******************************************************************************/
void SysTick_Handler(void)
{
	DoIrqService();					// The IRQ do main loop
}