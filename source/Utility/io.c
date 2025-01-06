	#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#define 	DEFAULT_UART		6
	#elif BOARD_ID ==  BOARD_ID_STM32F411E
#define 	DEFAULT_UART 		1
	#else // BOARD_ID == ?
#define 	DEFAULT_UART		1
	#endif // BOARD_ID == ?
	

typedef enum 
{
	USE_UART1 = 1,
	USE_UART2 = 2,
	USE_UART3 = 3,
	USE_UART4 = 4,
	USE_UART5 = 5,
	USE_UART6 = 6,
} use_uart;

static use_uart eUart = DEFAULT_UART;


static void ut_Init();

static void ut_InitUart();
static void ut_SendUart();


static void ut_InitUart()
{
	switch (eUart)
	{
		case USE_UART1:
			mcu_InitUsart1();
			break;
		case USE_UART2:
			mcu_InitUsart2();
			break;
		case USE_UART3:
		case USE_UART4:
		case USE_UART5:
			// Current there is no support for UART3/4/5
			break;
		case USE_UART6:
			mcu_InitUsart6();
			break;
	}
}


static void ut_SendUart(const char *pTxBuffer)
{
	switch (eUart)
	{
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
	}
}


static void ut_Init()
{
	ut_InitUart();
}