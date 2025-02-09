#include "main.h"

#define FPU 16000000

int main(void)
{

	ut_Init();		// Initialize independent utility such as UART, I2C, Timer, ...

	if ( Loop )
		is_SetIrqService( Loop );		// If Loop function is found declared in any application, 	setup IRQ accordingly
	else
		is_SetIrqService( DoNothing );

	if ( Init )
		Init(); 		// If Init function is found declared in any application, initialize project accordingly.

	while(1)
	{
		// Do nothing. 
		// Loop is done by timer interrupt
	}
}


// This function to assign pointer function in case we don't need to do anything while avoid assigning pointer to NULL which is dangerous
void DoNothing(void)
{
	// Do nothing
}