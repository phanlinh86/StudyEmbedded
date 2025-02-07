	#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
#define 	DEFAULT_UART		6
#define 	DEFAULT_I2C			1
	#elif BOARD_ID ==  BOARD_ID_STM32L010RB
#define 	DEFAULT_UART 		2
#define 	DEFAULT_I2C			1
	#elif BOARD_ID ==  BOARD_ID_ATMEGA328P
#define 	DEFAULT_UART 		0
#define 	DEFAULT_I2C			1
	#else // BOARD_ID == ?
#define 	DEFAULT_UART		1
#define 	DEFAULT_I2C			1
	#endif // BOARD_ID == ?
	

#define TIMER_PERIOD_IN_MS	1

uint8_t uart_rx_buffer[RX_USART_BUFFER];
uint8_t uart_tx_buffer[TX_USART_BUFFER];
bool bUartRxComplete = TRUE;

static void ut_Init();
static void ut_InitTimer();

typedef enum 
{
		#if ( BOARD_ID == BOARD_ID_STM32F411E )			
	USE_UART1 = 1,
	USE_UART2 = 2,
	USE_UART6 = 6,
		#elif ( BOARD_ID == BOARD_ID_STM32L010RB )
	USE_UART2 = 2,		
		#elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
	USE_UART0 = 0,			
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

    #if ( BOARD_ID == BOARD_ID_STM32F411E )
// I2C
typedef enum 
{
	USE_I2C1 = 1,
	USE_I2C2 = 2,
	USE_I2C3 = 3,			
} use_i2c;

static use_i2c eI2c = DEFAULT_I2C;

static void ut_InitI2c(uint8_t u8_DeviceAddress);
void ut_SendI2c(const uint8_t *pTxBuffer, uint8_t SlaveAddr);
uint8_t ut_ReadI2c(uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_Len);
void ut_WriteI2c(uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_SlaveValue);

// Accelerometer / Magnetometer / Temperature Sensor
static void ut_InitAccel(); 			// Initialize Accelerometer
static uint16_t ut_ReadTemp(void);		// Read temperature
    #endif // BOARD_ID ==  BOARD_ID_STM32F411E

static void ut_Init()
{
        #if ( BOARD_ID == BOARD_ID_STM32F411E )
	ut_InitI2c(ACCEL_SENSOR_SLAVE_ADDR);    // Initialize I2C
	ut_InitAccel();							// Initialize Accelerometer
	    #endif // BOARD_ID ==  BOARD_ID_STM32F411E
	ut_InitUart(); 		// Initialize UART
	ut_InitTimer();	    // Initialize timer for scheduler
}

static void ut_InitTimer()
{
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_STM32L010RB ) )
	mcu_ConfigSysTick(TIMER_PERIOD_IN_MS);					// STM32 F411E using systick as system timer
		#elif ( BOARD_ID ==  BOARD_ID_ATMEGA328P )
	is_InitTimer1(TIMER_PERIOD_IN_MS);				// Arduino using Timer1 as system timer
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E
}

static void ut_InitUart()
{
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_STM32F411E )
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
		case USE_UART6:
			mcu_InitUsart6();
			is_SetUsart6Interrupt();
			mcu_Usart6InitBuffer(uart_rx_buffer);
			is_InitUsart6Isr(mcu_Usart6IrqService);
			break;
				#elif ( BOARD_ID == BOARD_ID_STM32L010RB )
		case USE_UART2:
			mcu_InitUsart2();
			is_SetUsart2Interrupt();
			mcu_Usart2InitBuffer(uart_rx_buffer);
			is_InitUsart2Isr(mcu_Usart2IrqService);
			break;
				#elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			mcu_InitUsart0();
			is_SetUsart0Interrupt();
			mcu_Usart0InitBuffer(uart_rx_buffer);
			is_InitUsart0Isr(mcu_Usart0IrqService);
			break;			
				#endif // BOARD_ID == ?
	}
}


void ut_SendUart(const uint8_t *pTxBuffer)
{
	switch (eUart)
	{
				#if ( BOARD_ID == BOARD_ID_STM32F411E )
		case USE_UART1:
			mcu_Usart1SendData(pTxBuffer);
			break;
		case USE_UART2:
			mcu_Usart2SendData(pTxBuffer);
			break;
		case USE_UART6:
			mcu_Usart6SendData(pTxBuffer);
			break;
				#elif ( BOARD_ID == BOARD_ID_STM32L010RB )
		case USE_UART2:
			mcu_Usart2SendData(pTxBuffer);
			break;					
				#elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			mcu_Usart0SendData(pTxBuffer);
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
				#if ( BOARD_ID == BOARD_ID_STM32F411E )
		case USE_UART1:
			UsartConfig = mcu_GetUsart1Config();
			break;
		case USE_UART2:
			UsartConfig = mcu_GetUsart2Config();
			break;
		case USE_UART6:
			UsartConfig = mcu_GetUsart6Config();
			break;
				#elif ( BOARD_ID == BOARD_ID_STM32L010RB )
		case USE_UART2:
			UsartConfig = mcu_GetUsart2Config();
			break;			
				#elif ( BOARD_ID == BOARD_ID_ATMEGA328P )
		case USE_UART0:
			UsartConfig = mcu_GetUsart0Config();
			break;			
				#endif // BOARD_ID == ?
	}
	return UsartConfig;
}

    #if ( BOARD_ID == BOARD_ID_STM32F411E )
static void ut_InitI2c( uint8_t u8_DeviceAddress )
{
	switch (eI2c)
	{

		case USE_I2C1:
			mcu_InitI2c1(u8_DeviceAddress);
			break;
		case USE_I2C2:
			mcu_InitI2c2(u8_DeviceAddress);
			break;
		case USE_I2C3:
			mcu_InitI2c3(u8_DeviceAddress);
			break;
	}
}

void ut_SendI2c(const uint8_t *pTxBuffer, uint8_t SlaveAddr)
{
	switch (eI2c)
	{
		case USE_I2C1:
			mcu_I2c1SendData(pTxBuffer, SlaveAddr);
			break;
		case USE_I2C2:
			mcu_I2c2SendData(pTxBuffer, SlaveAddr);
			break;
		case USE_I2C3:
			mcu_I2c3SendData(pTxBuffer, SlaveAddr);
			break;
	}
}

uint8_t ut_ReadI2c(uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_Len)
{
	uint8_t u8_SlaveValue;
	switch (eI2c)
	{
		case USE_I2C1:
			u8_SlaveValue = mcu_u8_ReadI2c1(u8_DeviceAddress, u8_SlaveAddr, u8_Len);
			break;
		case USE_I2C2:
			u8_SlaveValue = mcu_u8_ReadI2c2(u8_DeviceAddress, u8_SlaveAddr, u8_Len);
			break;
		case USE_I2C3:
			u8_SlaveValue = mcu_u8_ReadI2c3(u8_DeviceAddress, u8_SlaveAddr, u8_Len);
			break;
	}
	return u8_SlaveValue;
}

void ut_WriteI2c(uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_SlaveValue)
{
	switch (eI2c)
	{
		case USE_I2C1:
			mcu_WriteI2c1(u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
			break;
		case USE_I2C2:
			mcu_WriteI2c2(u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
			break;
		case USE_I2C3:
			mcu_WriteI2c3(u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
			break;
	}
}

// Accelerometer / Magnetometer / Temperature Sensor
static void ut_InitAccel(void)
{
	mcu_WriteI2c1(ACCEL_SENSOR_SLAVE_ADDR, CTRL_REG1_A_REG, 0x77);	// ODR = 7. Enable all accelerators
	mcu_WriteI2c1(ACCEL_SENSOR_SLAVE_ADDR, CTRL_REG4_A_REG, 0x08);	// BDU = 1. HR=1. FS=0	
}
    #endif // BOARD_ID == ?