//
// Created by Phan Linh on 25/12/2024.
//

#include "BlinkLed.h"

static uint32_t u32_ButtonPressCount = 0;
usart_handle usart2_handle;

void Init(void)
{
	InitUsart2();
    InitLed();
    InitButtonInterrupt();
}

void Loop(void)
{
    BlinkLed();
}

void InitUsart2(void)
{
	usart2_handle.pUSARTx = USART2;
	usart2_handle.USART_Config.u32_BaudRate = USART_BAUDRATE_9600;
	usart2_handle.USART_Config.eMode = USART_MODE_ONLY_TX;
	usart2_handle.USART_Config.eNoOfStopBits = USART_NO_STOP_BITS_1p0;
	usart2_handle.USART_Config.u8_WordLength = USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.eParityControl = USART_PARITY_DISABLE;
	mcu_UsartInit(&usart2_handle);
	mcu_SetGpioAlternate("A2"); 	// USART2 TX
	mcu_SetGpioAlternate("A3"); 	// USART3 RX
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
	if (u32_ButtonPressCount % 2 == 1)
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
			mcu_UsartSendData(&usart2_handle, "Cut sieu khin. Thai Lan thua 2-3");
			eLed = GREEN;
		}
		Delay(200000);
		// mcu_UsartSendData(&usart2_handle, "Green", 5);
	}
}

void ButtonInterrupt(void)
{
	u32_ButtonPressCount++;
	is_ClearExtiPendingInterrupt(BUTTON);
	Delay(20000); 		// Small delay to avoid button bouncing
}
