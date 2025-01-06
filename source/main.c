#include "main.h"

#define FPU 16000000

int main(void)
{
	ut_Init();		// Initialize utility such as UART
	
	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{
		Loop();		// Main loop running project
	}
}