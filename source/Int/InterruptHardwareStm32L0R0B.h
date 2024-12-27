//
// Created by Phan Linh on 25/12/2024.
//

#ifndef INTERRUPTHARDWARESTM32F411E_H
#define INTERRUPTHARDWARESTM32F411E_H

// Timer related function
void DelayUs( uint32_t u32_TimeInUs );
void DelayMs( uint32_t u32_TimeInMs );
void Delay( uint32_t u32_DelayLoop );


// External settable interrupt handlers 
void EXTI0_1_IRQHandler( void);
void EXTI2_3_IRQHandler( void);
void EXTI4_15_IRQHandler( void);

// Function pointer for external interrupt handling
static void ( *pfServiceExti01Irq ) (void) ; 		// Pointer to handle interrupt EXTI0-1
static void ( *pfServiceExti23Irq ) (void) ; 		// Pointer to handle interrupt EXTI2-3
static void ( *pfServiceExti415Irq ) (void) ; 		// Pointer to handle interrupt EXTI4-15


// 
static void inline is_EnableExtiInterrupt();
static void inline is_SetExtiInterrupt(char* pinString);
static void inline is_ClearExtiPendingInterrupt(char* pinString);
static void InitIsr(char* pinString, void (*pfServiceFunction)(void));


#endif //INTERRUPTHARDWARESTM32F411E_H
