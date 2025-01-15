//
// Created by Phan Linh on 25/12/2024.
//

#ifndef INTERRUPTHARDWARESTM32F411E_H
#define INTERRUPTHARDWARESTM32F411E_H

// External settable interrupt handlers 
void EXTI0_IRQHandler( void );
void EXTI1_IRQHandler( void );
void EXTI2_IRQHandler( void );
void EXTI3_IRQHandler( void );
void EXTI4_IRQHandler( void );
void EXTI5_IRQHandler( void );
void EXTI6_IRQHandler( void );
void EXTI7_IRQHandler( void );
void EXTI8_IRQHandler( void );
void EXTI9_IRQHandler( void );
void EXTI10_IRQHandler( void );
void EXTI11_IRQHandler( void );
void EXTI12_IRQHandler( void );
void EXTI13_IRQHandler( void );
void EXTI14_IRQHandler( void );
void EXTI15_IRQHandler( void );
void USART1_IRQHandler( void );
void USART2_IRQHandler( void );
void USART6_IRQHandler( void );
void SysTick_Handler(void);

// Function pointer for external interrupt handling
static void ( *pfServiceExti0Irq ) (void) ; 		// Pointer to handle interrupt EXTI0
static void ( *pfServiceExti1Irq ) (void) ; 		// Pointer to handle interrupt EXTI1
static void ( *pfServiceExti2Irq ) (void) ; 		// Pointer to handle interrupt EXTI2
static void ( *pfServiceExti3Irq ) (void) ; 		// Pointer to handle interrupt EXTI3
static void ( *pfServiceExti4Irq ) (void) ; 		// Pointer to handle interrupt EXTI4
static void ( *pfServiceExti5Irq ) (void) ; 		// Pointer to handle interrupt EXTI5
static void ( *pfServiceExti6Irq ) (void) ; 		// Pointer to handle interrupt EXTI6
static void ( *pfServiceExti7Irq ) (void) ; 		// Pointer to handle interrupt EXTI7
static void ( *pfServiceExti8Irq ) (void) ; 		// Pointer to handle interrupt EXTI8
static void ( *pfServiceExti9Irq ) (void) ; 		// Pointer to handle interrupt EXTI9
static void ( *pfServiceExti10Irq ) (void) ; 		// Pointer to handle interrupt EXTI10
static void ( *pfServiceExti11Irq ) (void) ; 		// Pointer to handle interrupt EXTI11
static void ( *pfServiceExti12Irq ) (void) ; 		// Pointer to handle interrupt EXTI12
static void ( *pfServiceExti13Irq ) (void) ; 		// Pointer to handle interrupt EXTI13
static void ( *pfServiceExti14Irq ) (void) ; 		// Pointer to handle interrupt EXTI14
static void ( *pfServiceExti15Irq ) (void) ; 		// Pointer to handle interrupt EXTI15

static void ( *pfServiceUsart1Irq ) (void) ; 		// Pointer to handle interrupt USART1
static void ( *pfServiceUsart2Irq ) (void) ; 		// Pointer to handle interrupt USART2 
static void ( *pfServiceUsart6Irq ) (void) ; 		// Pointer to handle interrupt USART6 


// 
static void inline is_EnableExtiInterrupt();
static void inline is_SetExtiInterrupt(char* pinString);
static void inline is_ClearExtiPendingInterrupt(char* pinString);
static void InitIsr(char* pinString, void (*pfServiceFunction)(void));

static void is_SetUsart1Interrupt(void);
static void is_InitUsart1Isr(void (*pfServiceFunction)(void));
static void is_SetUsart2Interrupt(void);
static void is_InitUsart2Isr(void (*pfServiceFunction)(void));
static void is_SetUsart6Interrupt(void);
static void is_InitUsart6Isr(void (*pfServiceFunction)(void));

#endif //INTERRUPTHARDWARESTM32F411E_H
