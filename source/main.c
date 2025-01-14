#include "main.h"

#define FPU 16000000

int main(void)
{
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
	ut_Init();		// Initialize utility such as UART, Timer, ...
		#endif // BOARD_ID ==  BOARD_ID_STM32F411

	is_SetIrqService( Loop );		// Set default Loop function to run every timer interrupt. 
									// Can modify this behavior in Init function

	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{	
		// Do nothing. 
		// Loop is done by timer interrupt
	}
}