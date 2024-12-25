#include "main.h"
#include <stdlib.h>

#define FPU 16000000

int main(void)
{
	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{
		Loop();		// Main loop running project
	}
}