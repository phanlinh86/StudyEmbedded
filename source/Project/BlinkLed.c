//
// Created by Phan Linh on 25/12/2024.
//

#include "BlinkLed.h"

static uint32_t u32_ButtonPressCount = 0;
	#if ( BOARD_ID ==  BOARD_ID_STM32F411E ) 
char uart_buffer[MAX_USART_BUFFER] = "Hello World\n";
	#endif // BOARD_ID ==  BOARD_ID_STM32F411E 
static uint32_t u32_LedPeriodInMs = 500; 	// Switch led every 0.5s 

void Init(void)
{	
    InitLed();
    InitButtonInterrupt();
}

void Loop(void)
{
    BlinkLed();
}

void InitLed(void)
{
	// Enable and set output to 4 LEDS
	mcu_SetGpioOutput( LED_GREEN );
	mcu_SetGpioOutput( LED_ORANGE );
	mcu_SetGpioOutput( LED_RED );
	mcu_SetGpioOutput( LED_BLUE );
}

void InitButtonInterrupt(void)
{
	// Enable and set input for PA0
	mcu_SetGpioInput( BUTTON );
	// Set EXTI0 interrupt for pin PA0
	is_SetExtiInterrupt(BUTTON);
	// Enable EXTI interrupt
	is_EnableExtiInterrupt();
    // Set EXTI Interrupt Service to ButtonInterrupt
    InitIsr( BUTTON, ButtonInterrupt);
}

void BlinkLed(void)
{
  	static LED eLed = GREEN; 		// State machine control which LED to toggle
	char buffer[30];
	if (u32_ButtonPressCount % 2 == 1)
	{	
			#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
		if ( is_ReadSysTickCounter() >= u32_LedPeriodInMs )
			#endif // BOARD_ID ==  BOARD_ID_STM32F411E
		{
			switch ( eLed ) 
			{
				case GREEN:
					mcu_ToggleGpio( LED_GREEN );
					break;
				case ORANGE:
					mcu_ToggleGpio( LED_ORANGE );
					break;
				case RED:
					mcu_ToggleGpio( LED_RED );
					break;
				case BLUE:
					mcu_ToggleGpio( LED_BLUE );
					break;						
			}
			eLed ++;
			
			if ( eLed >= NUMBER_SUPPORTED_LED )
			{
				eLed = GREEN;
			}
				#if ( BOARD_ID ==  BOARD_ID_STM32F411E ) 
			is_SetSysTickCounter(0);
			// ut_ReceiveUart(uart_buffer);
			sprintf(uart_buffer, "SysTick:%u\n", is_ReadSysTickCounter());
			ut_SendUart(uart_buffer);
				#else // BOARD_ID !=  BOARD_ID_STM32F411E 
			Delay(200000);
				#endif // BOARD_ID ==  ?
		}
	}
}

void ButtonInterrupt(void)
{
	u32_ButtonPressCount++;
	is_ClearExtiPendingInterrupt(BUTTON);
	Delay(20000); 		// Small delay to avoid button bouncing
}
