	#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#define 	DEFAULT_UART		6
	#elif BOARD_ID ==  BOARD_ID_STM32L010RB
#define 	DEFAULT_UART 		1
	#elif BOARD_ID ==  BOARD_ID_ATMEGA328P
#define 	DEFAULT_UART 		0
	#else // BOARD_ID == ?
#define 	DEFAULT_UART		1
	#endif // BOARD_ID == ?
	

#define TIMER_PERIOD_IN_MS	1

uint8_t uart_rx_buffer[RX_USART_BUFFER];
uint8_t uart_tx_buffer[TX_USART_BUFFER];
bool bUartRxComplete = TRUE;

static void ut_Init();
static void ut_InitTimer();

	#if ( BOARD_ID !=  BOARD_ID_STM32L010RB )
typedef enum 
{
		#if ( BOARD_ID == BOARD_ID_ATMEGA328P )
	USE_UART0 = 0,
		#else // BOARD_ID != BOARD_ID_ATMEGA328P
	USE_UART1 = 1,
	USE_UART2 = 2,
	USE_UART3 = 3,
	USE_UART4 = 4,
	USE_UART5 = 5,
	USE_UART6 = 6,
		#endif // BOARD_ID == ?
} use_uart;

static use_uart eUart = DEFAULT_UART;
cmd_status eCmdStatus = CMD_IDLE;

static void ut_InitUart();
void ut_SendUart();
void ut_ResetRxBuffer();
void ut_ResetTxBuffer();
static use_uart ut_u16_GetUsedUsart(void);
static usart_config ut_GetUsartConfig(void);
	#endif // BOARD_ID !=  BOARD_ID_STM32L010RB

static void ut_Init()
{
		#if ( BOARD_ID !=  BOARD_ID_STM32L010RB )
	ut_InitUart(); 		// Initialize UART
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E
	ut_InitTimer();	// Initialize timer for scheduler
}

static void ut_InitTimer()
{
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_STM32L010RB ) )
	mcu_ConfigSysTick(TIMER_PERIOD_IN_MS);					// STM32 F411E using systick as system timer
		#elif ( BOARD_ID ==  BOARD_ID_ATMEGA328P )
	is_InitTimer1(TIMER_PERIOD_IN_MS);				// Arduino using Timer1 as system timer
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E
}

	#if ( BOARD_ID !=  BOARD_ID_STM32L010RB )
static void ut_InitUart()
{
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			mcu_InitUsart0();
			is_SetUsart0Interrupt();
			mcu_Usart0InitBuffer(uart_rx_buffer);
			is_InitUsart0Isr(mcu_Usart0IrqService);
			break;
				#else // BOARD_ID != BOARD_ID_ATMEGA328P
		case USE_UART1:
			mcu_InitUsart1();
			is_SetUsart1Interrupt();
			mcu_Usart1InitBuffer(uart_rx_buffer);
			is_InitUsart1Isr(mcu_Usart1IrqService);
			break;
		case USE_UART2:
			mcu_InitUsart2();
			is_SetUsart2Interrupt();
			mcu_Usart2InitBuffer(uart_rx_buffer);
			is_InitUsart2Isr(mcu_Usart2IrqService);
			break;
		case USE_UART3:
		case USE_UART4:
		case USE_UART5:
			// Current there is no support for UART3/4/5
			break;
		case USE_UART6:
			mcu_InitUsart6();
			is_SetUsart6Interrupt();
			mcu_Usart6InitBuffer(uart_rx_buffer);
			is_InitUsart6Isr(mcu_Usart6IrqService);
			break;
				#endif // BOARD_ID == ?
	}
}


void ut_SendUart(const uint8_t *pTxBuffer)
{
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			mcu_Usart0SendData(pTxBuffer);
			break;
				#else // BOARD_ID != BOARD_ID_ATMEGA328P
		case USE_UART1:
			mcu_Usart1SendData(pTxBuffer);
			break;
		case USE_UART2:
			mcu_Usart2SendData(pTxBuffer);
			break;
		case USE_UART3:
		case USE_UART4:
		case USE_UART5:
			// Current there is no support for UART3/4/5
			break;
		case USE_UART6:
			mcu_Usart6SendData(pTxBuffer);
			break;
				#endif // BOARD_ID == ?
	}
}
	
void ut_ResetRxBuffer()
{
	for (int i=0; i<RX_USART_BUFFER; i++)
		uart_rx_buffer[i] = '\0';
}

void ut_ResetTxBuffer()
{
	for (int i=0; i<TX_USART_BUFFER; i++)
		uart_tx_buffer[i] = '\0';
}

static use_uart ut_u16_GetUsedUsart(void)
{
	return eUart;
}

static usart_config ut_GetUsartConfig(void)
{
	usart_config UsartConfig;
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			UsartConfig = mcu_GetUsart0Config();
			break;
				#else // BOARD_ID != BOARD_ID_ATMEGA328P
		case USE_UART1:
			UsartConfig = mcu_GetUsart1Config();
			break;
		case USE_UART2:
			UsartConfig = mcu_GetUsart2Config();
			break;
		case USE_UART3:
		case USE_UART4:
		case USE_UART5:
			// Current there is no support for UART3/4/5
			break;
		case USE_UART6:
			UsartConfig = mcu_GetUsart6Config();
			break;
				#endif // BOARD_ID == ?
	}
	return UsartConfig;
}
	#endif // BOARD_ID !=  BOARD_ID_STM32L010RB
		
