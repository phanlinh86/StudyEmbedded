#include "main.h"

#define FPU 16000000

int main(void)
{
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
	ut_Init();		// Initialize utility such as UART, Timer, ...
		#endif // BOARD_ID ==  BOARD_ID_STM32F411

	Init(); 		// Initialize project such as interrupts, clock, peripherals, etc.

	while(1)
	{
			#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
		cmd_DoCommandIsr(); 		// Serve command sent from USART
			#endif // BOARD_ID ==  BOARD_ID_STM32F411E, BOARD_ID_ATMEGA328P
		Loop();						// Main loop running project
	}
}