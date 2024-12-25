//
// Created by Phan Linh on 25/12/2024.
//

#include "InterruptHardwareStm32F411E.h"
#include <stdio.h>

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
	// Enable NVIC
	*NVIC_ISER0 |= 1 << 6;
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
			break;
		case 1:
			SYSCFG->SYSCFG_EXTICR1.EXTI1 = (port - 'A'); 	//  EXTI1
			break;
		case 2:
			SYSCFG->SYSCFG_EXTICR1.EXTI2 = (port - 'A'); 	//  EXTI2			
			break;
		case 3:
			SYSCFG->SYSCFG_EXTICR1.EXTI3 = (port - 'A'); 	//  EXTI3
			break;			
		case 4:
			SYSCFG->SYSCFG_EXTICR2.EXTI4 = (port - 'A'); 	//  EXTI4
			break;
		case 5:
			SYSCFG->SYSCFG_EXTICR2.EXTI5 = (port - 'A'); 	//  EXTI5
			break;
		case 6:
			SYSCFG->SYSCFG_EXTICR2.EXTI6 = (port - 'A'); 	//  EXTI6
			break;			
		case 7:
			SYSCFG->SYSCFG_EXTICR2.EXTI7 = (port - 'A'); 	//  EXTI7
			break;
		case 8:
			SYSCFG->SYSCFG_EXTICR3.EXTI8 = (port - 'A'); 	//  EXTI8
			break;
		case 9:
			SYSCFG->SYSCFG_EXTICR3.EXTI9 = (port - 'A'); 	//  EXTI9
			break;
		case 10:
			SYSCFG->SYSCFG_EXTICR3.EXTI10 = (port - 'A'); 	//  EXTI10
			break;
		case 11:
			SYSCFG->SYSCFG_EXTICR3.EXTI11 = (port - 'A'); 	//  EXTI11
			break;
		case 12:
			SYSCFG->SYSCFG_EXTICR4.EXTI12 = (port - 'A'); 	//  EXTI12
			break;			
		case 13:
			SYSCFG->SYSCFG_EXTICR4.EXTI13 = (port - 'A'); 	//  EXTI13
			break;			
		case 14:
			SYSCFG->SYSCFG_EXTICR4.EXTI14 = (port - 'A'); 	//  EXTI14
			break;
		case 15:
			SYSCFG->SYSCFG_EXTICR4.EXTI15 = (port - 'A'); 	//  EXTI15
			break;
	} 
	// Disable EXTI0 Interrupt Masking
	EXTI->EXTI_IMR.Register |= ( 1 << exti );
	// Set rising edge detection
	EXTI->EXTI_RTSR.Register |= ( 1 << exti );
}


void EXTI0_IRQHandler(void)
{
	(*pfServiceExti0Irq)();
}
