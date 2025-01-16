#include "main.h"

#define FPU 16000000

int main(void)
{	
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