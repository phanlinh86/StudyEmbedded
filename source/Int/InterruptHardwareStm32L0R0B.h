//
// Created by Phan Linh on 25/12/2024.
//

#ifndef INTERRUPTHARDWARESTM32F411E_H
#define INTERRUPTHARDWARESTM32F411E_H

// External settable interrupt handlers 
void EXTI0_1_IRQHandler( void);
void EXTI2_3_IRQHandler( void);
void EXTI4_15_IRQHandler( void);
void SysTick_Handler(void);
void USART2_IRQHandler( void );

// Function pointer for external interrupt handling
static void ( *pfServiceExti01Irq ) (void) ; 		// Pointer to handle interrupt EXTI0-1
static void ( *pfServiceExti23Irq ) (void) ; 		// Pointer to handle interrupt EXTI2-3
static void ( *pfServiceExti415Irq ) (void) ; 		// Pointer to handle interrupt EXTI4-15
static void ( *pfServiceUsart2Irq ) (void) ; 		// Pointer to handle interrupt USART2 

// 
static void inline is_EnableExtiInterrupt();
static void inline is_SetExtiInterrupt(char* pinString);
static void inline is_ClearExtiPendingInterrupt(char* pinString);
static void InitIsr(char* pinString, void (*pfServiceFunction)(void));


static void is_SetUsart2Interrupt(void);
static void is_InitUsart2Isr(void (*pfServiceFunction)(void));

#endif //INTERRUPTHARDWARESTM32F411E_H
