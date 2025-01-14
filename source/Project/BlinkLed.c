//
// Created by Phan Linh on 25/12/2024.
//

#include "BlinkLed.h"

uint32_t u32_ButtonPressCount = 0;
	#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
uint8_t uart_rx_buffer[RX_USART_BUFFER];
uint8_t uart_tx_buffer[TX_USART_BUFFER];
	#endif // BOARD_ID ==  BOARD_ID_STM32F411E

uint32_t u32_LedPeriodInMs = BLINK_LED_PERIOD_IN_MS; 	// Toggle LED in this period/2


void Init(void)
{	
    InitLed();
		#if ( BOARD_ID != BOARD_ID_ATMEGA328P )
    InitButtonInterrupt();
		#endif // BOARD_ID != BOARD_ID_ATMEGA328P
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

	#if ( BOARD_ID != BOARD_ID_ATMEGA328P )
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
	#endif // BOARD_ID != BOARD_ID_ATMEGA328P

void BlinkLed(void)
{
  	static LED eLed = GREEN; 		// State machine control which LED to toggle
	if (u32_ButtonPressCount % 2 == 0)
	{	
			#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
		if ( is_ReadSysTickCounter() >= u32_LedPeriodInMs )
		    #elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
		if ( is_u32_ReadTimer1Counter() >= u32_LedPeriodInMs )
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
			is_SetSysTickCounter(0);        // Use SysTick to control LED blink
			// ut_ReceiveUart(uart_buffer);
			sprintf((char*) uart_tx_buffer, "SysTick:%lu\n", is_ReadSysTickCounter());
			ut_SendUart(uart_tx_buffer);
				#elif ( BOARD_ID ==  BOARD_ID_STM32L010RB )
			Delay(200000);                  // Use Timer0 to control LED blink
			    #elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
			//sprintf((char*) uart_tx_buffer, "Status:%lu %lu %lu %lu \n", cmd_GetRespStatus(),cmd_GetResp(0), cmd_GetResp(1), cmd_GetResp(2));			
			is_SetTimer1Counter(0);        // Use Timer0 to control LED blink
				#endif // BOARD_ID ==  ?
		}
	}
}

	#if ( BOARD_ID != BOARD_ID_ATMEGA328P )
void ButtonInterrupt(void)
{
	u32_ButtonPressCount++;
	is_ClearExtiPendingInterrupt(BUTTON);
	Delay(20000); 		// Small delay to avoid button bouncing
}
	#endif // BOARD_ID != BOARD_ID_ATMEGA328P
