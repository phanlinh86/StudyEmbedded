#include "main.h"

#define FPU 16000000

int main(void)
{
		#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
	ut_Init();		// Initialize utility such as UART, SysTick
		#endif // BOARD_ID ==  BOARD_ID_STM32F411

	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{
		Loop();		// Main loop running project
	}
}