	#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#define 	DEFAULT_UART		1
	#elif BOARD_ID ==  BOARD_ID_STM32L010RB
#define 	DEFAULT_UART 		1
	#elif BOARD_ID ==  BOARD_ID_ATMEGA328P
#define 	DEFAULT_UART 		0
	#else // BOARD_ID == ?
#define 	DEFAULT_UART		1
	#endif // BOARD_ID == ?
	
#define MAX_USART_BUFFER 		100
#define SYS_TICK_PERIOD_IN_MS	1

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
extern char uart_buffer[MAX_USART_BUFFER];


static void ut_Init();

static void ut_InitUart();
static void ut_SendUart();
// static void ut_ReceiveUart();

static void ut_InitTimer();


static void ut_InitUart()
{
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			mcu_InitUsart0();
			is_SetUsart0Interrupt();
			mcu_Usart0InitBuffer(uart_buffer);
			is_InitUsart0Isr(mcu_Usart0IrqService);
			break;
				#else // BOARD_ID != BOARD_ID_ATMEGA328P
		case USE_UART1:
			mcu_InitUsart1();
			is_SetUsart1Interrupt();
			mcu_Usart1InitBuffer(uart_buffer);
			is_InitUsart1Isr(mcu_Usart1IrqService);
			break;
		case USE_UART2:
			mcu_InitUsart2();
			is_SetUsart2Interrupt();
			mcu_Usart2InitBuffer(uart_buffer);
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
			mcu_Usart6InitBuffer(uart_buffer);
			is_InitUsart6Isr(mcu_Usart6IrqService);
			break;
				#endif // BOARD_ID == ?
	}
}


static void ut_SendUart(const char *pTxBuffer)
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
	#endif // BOARD_ID ==  BOARD_ID_STM32F411E
static void ut_InitTimer()
{
		#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
	mcu_ConfigSysTick(SYS_TICK_PERIOD_IN_MS);					// STM32 F411E using systick as system timer
		#elif ( BOARD_ID ==  BOARD_ID_ATMEGA328P )
	is_InitTimer1(SYS_TICK_PERIOD_IN_MS);				// Arduino using Timer1 as system timer
		#else
	// Do Nothing
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E
}

static void ut_Init()
{
		#if ( BOARD_ID !=  BOARD_ID_STM32L010RB )
	ut_InitUart(); 		// Initialize UART
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E
	ut_InitTimer();	// Initialize SysTick
}