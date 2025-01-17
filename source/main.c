#include "main.h"

#define FPU 16000000

int main(void)
{
        #if ( BOARD_ID == BOARD_ID_STM32L010RB )
    // Set the clock to HSI for higher speed 16 Mhz instead of 2.097 Mhz
    // Only for STM32L010RB
    // This code will need to be clean up later
	RCC->RCC_CR |= 1;
	while ( !( RCC->RCC_CR & 0x4 ) ) {};
	RCC->RCC_CFGR.SW = RCC_CLK_SRC_HSI;
	while ( RCC->RCC_CFGR.SWS != RCC_CLK_SRC_HSI ) {};
	    #endif // BOARD_ID == BOARD_ID_STM32L010RB
	ut_Init();		// Initialize utility such as UART, Timer, ...

	is_SetIrqService( Loop );		// Set default Loop function to run every timer interrupt. 
									// Can modify this behavior in Init function

	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{
		// Do nothing. 
		// Loop is done by timer interrupt
	}
}