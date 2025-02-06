//
// Created by Phan Linh on 25/12/2024.
//
#pragma once

typedef enum
{
	RCC_CLK_SRC_HSI 		= 0,
	RCC_CLK_SRC_HSE 		= 1,
	RCC_CLK_SRC_PLL 		= 2,
} rcc_clk_src;

typedef struct
{
	usart_regs 		*pUSARTx; 			// Pointer to go to USART register set
	usart_config   	USART_Config;		// Struct to store the config
}usart_handle;

typedef struct
{
	i2c_regs 		*pI2Cx;
	i2c_config 		I2C_Config;
	uint8_t 		*pTxBuffer; /* !< To store the app. Tx buffer address > */
	uint8_t 		*pRxBuffer;	/* !< To store the app. Rx buffer address > */
	uint32_t 		TxLen;		/* !< To store Tx len > */
	uint32_t 		RxLen;		/* !< To store Tx len > */
	uint8_t 		TxRxState;	/* !< To store Communication state > */
	uint8_t 		DevAddr;	/* !< To store slave/device address > */
    uint32_t        RxSize;		/* !< To store Rx size  > */
    uint8_t         Sr;			/* !< To store repeated start value  > */
}i2c_handle;


static usart_handle usart1_handle;
static usart_handle usart2_handle;
static usart_handle usart6_handle;

static uint8_t* usart1_buffer;
static uint8_t* usart2_buffer;
static uint8_t* usart6_buffer;
static uint8_t usart1_buffer_len = 0;
static uint8_t usart2_buffer_len = 0;
static uint8_t usart6_buffer_len = 0;

static i2c_handle i2c1_handle;
static i2c_handle i2c2_handle;
static i2c_handle i2c3_handle;
static uint8_t* i2c1_buffer;
static uint8_t* i2c2_buffer;
static uint8_t* i2c3_buffer;

// GPIO
static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline uint8_t mcu_u8_ReadGpio( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );
static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue );

// USART1
static void mcu_InitUsart1();
static void mcu_Usart1SendData(const uint8_t *pTxBuffer);
static void mcu_Usart1InitBuffer(uint8_t *pBuffer);
static void mcu_Usart1IrqService(void);
static usart_config mcu_GetUsart1Config(void);

// USART2
static void mcu_InitUsart2();
static void mcu_Usart2SendData(const uint8_t *pTxBuffer);
static void mcu_Usart2InitBuffer(uint8_t *pBuffer);
static void mcu_Usart2IrqService(void);
static usart_config mcu_GetUsart2Config(void);

// USART6
static void mcu_InitUsart6();
static void mcu_Usart6SendData(const uint8_t *pTxBuffer);
static void mcu_Usart6InitBuffer(uint8_t *pBuffer);
static void mcu_Usart6IrqService(void);
static usart_config mcu_GetUsart6Config(void);


// USART
static inline void mcu_UsartInit( usart_handle* pUsartHandle ); 		// Enable and initiliaze USART
static inline void mcu_UsartSetBaudRate( usart_handle *pUSARTHandle );	// Set USART baud rate
void mcu_UsartSendData(usart_handle *pUSARTHandle, const uint8_t *pTxBuffer);
void mcu_UsartSendChar(usart_handle *pUSARTHandle, uint8_t data);
static uint8_t mcu_UsartDataAvailable(usart_handle *pUSARTHandle);
void mcu_UsartReceiveData(usart_handle *pUSARTHandle, uint8_t *str);
uint8_t mcu_UsartGetChar(usart_handle *pUSARTHandle);


// Clock
static uint32_t mcu_u32_ReadSysClk();
static uint32_t mcu_u32_ReadAhbClk();
static uint32_t mcu_u32_ReadClkApb1();
static uint32_t mcu_u32_ReadClkApb2();

static uint32_t mcu_u32_ReadSysTickClk();
static uint32_t mcu_u32_ReadStkVal();
static void mcu_SetStkVal( uint32_t val);
static uint32_t mcu_u32_ReadStkReload();
static void mcu_SetStkReload( uint32_t val);
static void mcu_SetStkInterrupt( bool bEnable );
static void mcu_SetStkCounter( bool bEnable );
static void mcu_SetStkCalib( uint32_t val );
static void mcu_ConfigSysTick( uint32_t u32_TickInMs );

// I2C
static inline void mcu_I2cInit( i2c_handle* pI2CHandle ); 			// Enable and initiliaze I2C
static inline void mcu_u8_WriteI2cData( 	i2c_handle* pI2CHandle,
											uint8_t u8_DeviceAddr,
											uint8_t u8_SlaveAddr,
											uint8_t u8_SlaveValue);
static inline uint8_t mcu_u8_ReadI2cData(	i2c_handle* pI2CHandle, 
											uint8_t u8_DeviceAddr, 
											uint8_t u8_SlaveAddr );
static inline void mcu_I2cMasterSendData( i2c_handle* pI2CHandle, const uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr ); 		// Send data through I2C
static inline void mcu_I2cMasterReceiveData( i2c_handle* pI2CHandle, uint8_t *pTxbuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr ); 		// Receive data through I2C
static void mcu_ClearI2cAddrFlag(i2c_handle *pI2CHandle );

// I2C1
static void mcu_InitI2c1(uint8_t u8_DeviceAddress);
static void mcu_I2c1SendData(const uint8_t *pTxBuffer, uint8_t SlaveAddr);
static uint8_t mcu_u8_ReadI2c1(uint8_t u8_DeviceAddr, uint8_t SlaveAddr);
static void mcu_WriteI2c1(uint8_t u8_DeviceAddr, uint8_t SlaveAddr, uint8_t u8_SlaveValue);
static void mcu_I2c1InitBuffer(uint8_t *pBuffer);
static void mcu_I2c1IrqService(void);
// I2C2
static void mcu_InitI2c2(uint8_t u8_DeviceAddress);
static void mcu_I2c2SendData(const uint8_t *pTxBuffer, uint8_t SlaveAddr);
static uint8_t mcu_u8_ReadI2c2(uint8_t u8_DeviceAddr, uint8_t SlaveAddr);
static void mcu_WriteI2c2(uint8_t u8_DeviceAddr, uint8_t SlaveAddr, uint8_t u8_SlaveValue);
static void mcu_I2c2InitBuffer(uint8_t *pBuffer);
static void mcu_I2c2IrqService(void);

// I2C3
static void mcu_InitI2c3(uint8_t u8_DeviceAddress);
static uint8_t mcu_u8_ReadI2c3(uint8_t u8_DeviceAddr, uint8_t SlaveAddr);
static void mcu_WriteI2c3(uint8_t u8_DeviceAddr, uint8_t SlaveAddr, uint8_t u8_SlaveValue);
static void mcu_I2c3SendData(const uint8_t *pTxBuffer, uint8_t SlaveAddr);
static void mcu_I2c3InitBuffer(uint8_t *pBuffer);
static void mcu_I2c3IrqService(void);

// Reset 
static inline void mcu_SoftReset();

__attribute__((always_inline)) static inline void __DSB(void)
{
  __asm volatile ("dsb 0xF":::"memory");
}


/********************************************************************************
*									STM32 GPIOA_BASE							*
*									General Purpose I/O							*
*********************************************************************************/

static inline void mcu_SetGpioOutput( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	RCC->RCC_AHB1ENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 1);
}

static inline void mcu_SetGpioInput( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
  	RCC->RCC_AHB1ENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 0);
}

static inline void mcu_ToggleGpio( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register ^= ( 1 << pin );			// Set GPIO mode to output
}

static inline void mcu_SetGpioHigh( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register |= ( 1 << pin );			// Set GPIO mode to output
}

static inline void mcu_SetGpioLow( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register &= ~( 1 << pin );			// Set GPIO mode to output
}

static inline uint8_t mcu_u8_ReadGpio( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	if ( BITFIELDMNGET(GPIO->MODER.Register, pin*2+1, pin*2) == 1 )
		return ( ( GPIO->ODR.Register >> pin ) & 0x01 );			// Set GPIO mode to output
	else
		return ( ( GPIO->IDR.Register >> pin ) & 0x01 );			// Set GPIO mode to output
}

static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	RCC->RCC_AHB1ENR.Register |= ( 1 << ( port - 'A' ) ); 		// Enable GPIO clock		
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 2); 	// Set moder to 2 for alternate function
	BITFIELDMNSET(GPIO->PUPDR.Register, pin*2+1, pin*2, 1);		// Set pull up register for correct start bit	
	if ( pin < 8 )
		BITFIELDMNSET(GPIO->AFRL.Register, pin*4+3, pin*4, u8_AlternateValue);	// Set Alternate function
	else
		BITFIELDMNSET(GPIO->AFRH.Register, (pin-8)*4+3, (pin-8)*4, u8_AlternateValue);	// Set Alternate function
}


/********************************************************************************
 * 									STM32 USART_BASE							*
 * 						Universal Synchronous Asynchronous Receiver Transmitter	*
 * ******************************************************************************/
void mcu_InitUsart1(void)
{	
	usart1_handle.pUSARTx 						= USART1;
	usart1_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_500000;
	usart1_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart1_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart1_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart1_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart1_handle);
	mcu_SetGpioAlternate("B6", 7); 	// USART1 TX. Use B6 instead of A9 due to A9 connected to a LED with capacitor
	mcu_SetGpioAlternate("B7", 7); 	// USART1 RX. Use B6 instead of A9 due to A10 connected to a LED with capacitor
}

static void mcu_Usart1SendData(const uint8_t *pTxBuffer)
{
	mcu_UsartSendData( &usart1_handle, pTxBuffer );
}

static void mcu_Usart1InitBuffer(uint8_t *pBuffer)
{
	usart1_buffer = pBuffer;
}

static void mcu_Usart1IrqService(void)
{
	*(usart1_buffer + usart1_buffer_len)  = mcu_UsartGetChar(&usart1_handle);
	if ( usart1_buffer_len >= RX_USART_BUFFER - 1 )
	{
		usart1_buffer_len = 0;
		bUartRxComplete = TRUE;
	}
	else
	{
		usart1_buffer_len++;
		bUartRxComplete = FALSE;
	}
}


void mcu_InitUsart2(void)
{	
	usart2_handle.pUSARTx 						= USART2;
	usart2_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_500000;
	usart2_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart2_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart2_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart2_handle);
	mcu_SetGpioAlternate("A2", 7); 	// USART2 TX
	mcu_SetGpioAlternate("A3", 7); 	// USART3 RX	
}

static void mcu_Usart2SendData(const uint8_t *pTxBuffer)
{
	mcu_UsartSendData( &usart2_handle, pTxBuffer );
}

static void mcu_Usart2InitBuffer(uint8_t *pBuffer)
{
	usart2_buffer = pBuffer;
}

static void mcu_Usart2IrqService(void)
{
	*(usart2_buffer + usart2_buffer_len) = mcu_UsartGetChar(&usart2_handle);
	if ( usart2_buffer_len >= RX_USART_BUFFER - 1 )
	{
		usart2_buffer_len = 0;
		bUartRxComplete = TRUE;
	}
	else
	{
		usart2_buffer_len++;
		bUartRxComplete = FALSE;
	}
}

void mcu_InitUsart6(void)
{	
	usart6_handle.pUSARTx 						= USART6;
	usart6_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_500000;
	usart6_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart6_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart6_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart6_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart6_handle);
	mcu_SetGpioAlternate("C6", 8); 	// USART6 TX.
	mcu_SetGpioAlternate("C7", 8); 	// USART7 RX.
}

static void mcu_Usart6SendData(const uint8_t *pTxBuffer)
{
	mcu_UsartSendData( &usart6_handle, pTxBuffer );
}

static void mcu_Usart6InitBuffer(uint8_t *pBuffer)
{
	usart6_buffer = pBuffer;
}

static void mcu_Usart6IrqService(void)
{
	*(usart6_buffer + usart6_buffer_len)  = mcu_UsartGetChar(&usart6_handle);
	if ( usart6_buffer_len >= RX_USART_BUFFER - 1 )
	{
		usart6_buffer_len = 0;
		bUartRxComplete = TRUE;
	}
	else
	{
		usart6_buffer_len++;
		bUartRxComplete = FALSE;
	}
}
 
void mcu_UsartInit(usart_handle *pUSARTHandle)
{

	//Temporary variable
	uint32_t u32_TempReg = 0;

	/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given USART peripheral
	// TODO
	// USART1 / USART6 -> APB2
	// USART2 -> APB1
	
	if ( pUSARTHandle->pUSARTx == USART1 )
		RCC->RCC_APB2ENR.USART1EN = 1;
	else if ( pUSARTHandle->pUSARTx == USART2 )
		RCC->RCC_APB1ENR.USART2EN = 1;
	else if ( pUSARTHandle->pUSARTx == USART6 )
		RCC->RCC_APB2ENR.USART6EN = 1;

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	pUSARTHandle->pUSARTx->USART_CR1.UE = 1;
	if ( pUSARTHandle->USART_Config.eMode == USART_MODE_ONLY_RX)
	{
		pUSARTHandle->pUSARTx->USART_CR1.RE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.RXNEIE = 1;		// Enable RXNE interrupt
	}else if (pUSARTHandle->USART_Config.eMode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field 
		pUSARTHandle->pUSARTx->USART_CR1.TE = 1;

	}else if (pUSARTHandle->USART_Config.eMode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields 
		pUSARTHandle->pUSARTx->USART_CR1.TE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.RE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.RXNEIE = 1;		// Enable RXNE interrupt
	}

    //Implement the code to configure the Word length configuration item 
	pUSARTHandle->pUSARTx->USART_CR1.M = pUSARTHandle->USART_Config.u8_WordLength - 8;


    //Configuration of parity control bit fields
	if ( pUSARTHandle->USART_Config.eParityControl == USART_PARITY_EN_EVEN)
	{
		pUSARTHandle->pUSARTx->USART_CR1.PCE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.PS = 0;
	}
	else if (pUSARTHandle->USART_Config.eParityControl == USART_PARITY_EN_ODD )
	{
		pUSARTHandle->pUSARTx->USART_CR1.PCE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.PS = 1;
	}
	else // USART_PARITY_DISABLE
	{
		pUSARTHandle->pUSARTx->USART_CR1.PCE = 0;
		pUSARTHandle->pUSARTx->USART_CR1.PS = 0;		
	}

	// Set OVER8 = 1 to get higher speed
	// pUSARTHandle->pUSARTx->USART_CR1.OVER8 = 1;

	/******************************** Configuration of CR2******************************************/

	u32_TempReg=0;

	//Implement the code to configure the number of stop bits inserted during USART frame transmission 
	switch ( pUSARTHandle->USART_Config.eNoOfStopBits ) 
	{
		case USART_NO_STOP_BITS_0p5:
			pUSARTHandle->pUSARTx->USART_CR2.STOP = 1;
			break;
		case USART_NO_STOP_BITS_1p0:
			pUSARTHandle->pUSARTx->USART_CR2.STOP = 0;
			break;
		case USART_NO_STOP_BITS_1p5:
			pUSARTHandle->pUSARTx->USART_CR2.STOP = 3;
			break;
		case USART_NO_STOP_BITS_2p0:
			pUSARTHandle->pUSARTx->USART_CR2.STOP = 2;
			break;
	}
	// Set baud rate
	mcu_UsartSetBaudRate(pUSARTHandle);
}


static inline void mcu_UsartSetBaudRate( usart_handle *pUSARTHandle )
{
	uint32_t u32_ApbClk;	// APB Clock (USART1/USART6 on APB2 and USART2 on APB1)
	uint32_t USARTDIV;
	unsigned OVER8;


	//variables to hold Mantissa and Fraction values
	uint32_t M_part,F_part;
	if ( (pUSARTHandle->pUSARTx == USART1) || (pUSARTHandle->pUSARTx == USART6) )
	{
		u32_ApbClk = mcu_u32_ReadClkApb2();
	}
	else if (pUSARTHandle->pUSARTx == USART2)
	{
		u32_ApbClk = mcu_u32_ReadClkApb1();
	}
	OVER8 = pUSARTHandle->pUSARTx->USART_CR1.OVER8;
	// Formula from the specs
	// Tx/Rx baud = u32_ApbClk / ( 8 * (2-OVER8) * USARTDIV );
	// USARTDIV 	= u32_ApbClk / ( 8 * (2-OVER8) ) / Tx/Rx baud;
	USARTDIV = ( u32_ApbClk * 16 ) / ( 8 * (2-OVER8) ) / pUSARTHandle->USART_Config.u32_BaudRate;
	pUSARTHandle->pUSARTx->USART_BRR.Register = USARTDIV;

}

void mcu_UsartSendChar(usart_handle *pUSARTHandle, uint8_t data)
{
	while(!pUSARTHandle->pUSARTx->USART_SR.TXE);
	
	if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		pUSARTHandle->pUSARTx->USART_DR.DR = data & (uint16_t)0x01FF;
	else
		pUSARTHandle->pUSARTx->USART_DR.DR = data & (uint8_t)0xFF;
}

uint8_t mcu_UsartDataAvailable(usart_handle *pUSARTHandle)
{
	return pUSARTHandle->pUSARTx->USART_SR.RXNE;
}

uint8_t mcu_UsartGetChar(usart_handle *pUSARTHandle)
{
    // while(!pUSARTHandle->pUSARTx->USART_SR.RXNE); 	// Since this will be handled by interrupt
    if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			return (uint8_t)(pUSARTHandle->pUSARTx->USART_DR.DR & (uint16_t)0x01FF);
		else
			return (uint8_t)(pUSARTHandle->pUSARTx->USART_DR.DR & (uint8_t)0x0FF);
    else
		if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			return (uint8_t)(pUSARTHandle->pUSARTx->USART_DR.DR & (uint8_t)0xFF);
		else
			return (uint8_t)(pUSARTHandle->pUSARTx->USART_DR.DR  & (uint8_t)0x7F);
}

void mcu_UsartSendData(usart_handle *pUSARTHandle, const uint8_t *pTxBuffer)
{
	static uint8_t j=0;
	
	for (j=0; j<= TX_USART_BUFFER-1; j++)
		mcu_UsartSendChar(pUSARTHandle, pTxBuffer[j]);

	//Implement the code to wait till TC flag is set in the SR
	while(!pUSARTHandle->pUSARTx->USART_SR.TC);
}


void mcu_UsartReceiveData(usart_handle *pUSARTHandle, uint8_t *str)
{
    uint8_t data;
	int index;
	
	if ( mcu_UsartDataAvailable(pUSARTHandle) == 0 )
		return;
		
    do
    {
        data = mcu_UsartGetChar(pUSARTHandle);
		*str = data;
		if ( (pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS) && 
			 (pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE) )
			str += 2;
		else
			str++;
		index++;
		if ( index == 4 )
			break;
		
    } while (data != '\0');
}

static usart_config mcu_GetUsart1Config(void)
{
	return usart1_handle.USART_Config;
}

static usart_config mcu_GetUsart2Config(void)
{
	return usart2_handle.USART_Config;
}

static usart_config mcu_GetUsart6Config(void)
{
	return usart6_handle.USART_Config;
}


/********************************************************************************
 * 									STM32 RCC									*
 * 						Reset and Clock Control									*
 * ******************************************************************************/
static uint32_t mcu_u32_ReadSysClk()
{
	rcc_clk_src eRccClkSrc = RCC->RCC_CFGR.SWS; 
	
	switch (eRccClkSrc)
	{
		case RCC_CLK_SRC_HSI:
			return 16000000; 	// HSI using 16Mhz clock
		case RCC_CLK_SRC_HSE:
			return 16000000; 	// HSI using 16Mhz clock
		case RCC_CLK_SRC_PLL:
			return 48000000; 	// PLL using 48Mhz clock
	}
}

static uint32_t mcu_u32_ReadAhbClk()
{
	uint32_t u32_SysClk = mcu_u32_ReadSysClk();
	unsigned u16_AhbPreScale = RCC->RCC_CFGR.HPRE;
	
	if ( u16_AhbPreScale > 4 )
		u16_AhbPreScale = 1 << (u16_AhbPreScale - 3);
	else
		u16_AhbPreScale = 1;
	
	return u32_SysClk / u16_AhbPreScale;
}

static uint32_t mcu_u32_ReadClkApb1()
{
	unsigned u16_Apb1PreScale = RCC->RCC_CFGR.PPRE1;
	uint32_t u32_AhbClk = mcu_u32_ReadAhbClk();

	if ( u16_Apb1PreScale > 4 )
		u16_Apb1PreScale = 1 << (u16_Apb1PreScale - 3);
	else
		u16_Apb1PreScale = 1;
	
	return u32_AhbClk  / u16_Apb1PreScale;
	
}

static uint32_t mcu_u32_ReadClkApb2()
{
	unsigned u16_Apb2PreScale = RCC->RCC_CFGR.PPRE2;
	uint32_t u32_AhbClk = mcu_u32_ReadAhbClk();

	if ( u16_Apb2PreScale > 4 )
		u16_Apb2PreScale = 1 << (u16_Apb2PreScale - 3);
	else
		u16_Apb2PreScale = 1;

	return u32_AhbClk  / u16_Apb2PreScale;
	
}

/********************************************************************************
 * 									ARM SYS TICK								*
 * 								System Tick Timer								*
 * ******************************************************************************/
 
static uint32_t mcu_u32_ReadSysTickClk()
{
	uint32_t u32_AhbClk = mcu_u32_ReadAhbClk();
	
	if ( SYSTICK->STK_CTRL.CLKSOURCE )
		return u32_AhbClk;
	else
		return u32_AhbClk / 8;
}

static void mcu_SetStkClkSrc( uint8_t clkSrc )
{
	SYSTICK->STK_CTRL.CLKSOURCE = clkSrc;
}

static uint32_t mcu_u32_ReadStkVal()
{
	return SYSTICK->STK_VAL.CURRENT;
}

static void mcu_SetStkVal( uint32_t val)
{
	SYSTICK->STK_VAL.CURRENT = val;
}

static uint32_t mcu_u32_ReadStkReload()
{
	return SYSTICK->STK_LOAD.RELOAD;
}

static void mcu_SetStkReload( uint32_t val)
{
	SYSTICK->STK_LOAD.RELOAD = val;
}

static void mcu_SetStkInterrupt( bool bEnable )
{
	SYSTICK->STK_CTRL.TICKINT = bEnable;
}

static void mcu_SetStkEnable( bool bEnable )
{
	SYSTICK->STK_CTRL.ENABLE = bEnable;
}

static void mcu_SetStkCalib( uint32_t val )
{
	SYSTICK->STK_CALIB.TENMS = val;
}

static void mcu_ConfigSysTick( uint32_t u32_TickInMs )
{
	uint32_t u32_SysTickClk; 
	
	mcu_SetStkEnable(FALSE); 										// Temporary disable SysTick 
	mcu_SetStkClkSrc(1); 											// Set Clock to AHB
	u32_SysTickClk = mcu_u32_ReadSysTickClk(); 						// Current SysTick clock
	mcu_SetStkInterrupt(TRUE); 										// Enable interrupt 
	mcu_SetStkReload( u32_SysTickClk / 1000 * u32_TickInMs - 1 );	// Set value for Tick In Ms
	mcu_SetStkVal(0);												// Clear the current counter
	mcu_SetStkEnable(TRUE); 										// Temporary disable SysTick 
}

/********************************************************************************
 * 									STM32 I2C									*
 * 						Inter-integrated circuit Interface						*
 * ******************************************************************************/
// Enable and initiliaze I2C
static inline void mcu_I2cInit( i2c_handle* pI2CHandle )
{
	// Enable the clock for the I2C peripheral
	if ( pI2CHandle->pI2Cx == I2C1 )
		RCC->RCC_APB1ENR.I2C1EN = 1;
	else if ( pI2CHandle->pI2Cx == I2C2 )
		RCC->RCC_APB1ENR.I2C2EN = 1;
	else if ( pI2CHandle->pI2Cx == I2C3 )
		RCC->RCC_APB1ENR.I2C3EN = 1;	

	// ACK control bit
	pI2CHandle->pI2Cx->I2C_CR1.ACK = pI2CHandle->I2C_Config.u8_AckControl;

	// Configure FREQ
	pI2CHandle->pI2Cx->I2C_CR2.FREQ =  mcu_u32_ReadClkApb1() / 1000000U;

   // I2C Address (7-bit address mode). So will need to review again for 10-bit address here
	pI2CHandle->pI2Cx->I2C_OAR1.Register |= ( 1 << 14);
	pI2CHandle->pI2Cx->I2C_OAR1.ADD71 = pI2CHandle->I2C_Config.u8_Address;	// 7-bit address

	// CCR calculations	
	if(pI2CHandle->I2C_Config.u32_ClockSpeed <= I2C_SCL_SPEED_SM)
	{
		//Standard mode
		pI2CHandle->pI2Cx->I2C_CCR.FS = 0;
		pI2CHandle->pI2Cx->I2C_CCR.DUTY = 0;
		pI2CHandle->pI2Cx->I2C_CCR.CCR = (mcu_u32_ReadClkApb1() / ( 2 * pI2CHandle->I2C_Config.u32_ClockSpeed ) );
	}else
	{
		//Fast mode
		pI2CHandle->pI2Cx->I2C_CCR.FS = 1;
		pI2CHandle->pI2Cx->I2C_CCR.DUTY = pI2CHandle->I2C_Config.u8_DutyCycle;
		if(pI2CHandle->I2C_Config.u8_DutyCycle == I2C_FM_DUTY_2)
		{
			pI2CHandle->pI2Cx->I2C_CCR.CCR = (mcu_u32_ReadClkApb1() / ( 3 * pI2CHandle->I2C_Config.u32_ClockSpeed ) );
		}else
		{
			pI2CHandle->pI2Cx->I2C_CCR.CCR = (mcu_u32_ReadClkApb1() / ( 25 * pI2CHandle->I2C_Config.u32_ClockSpeed ) );
		}		
	}
	pI2CHandle->pI2Cx->I2C_CCR.CCR = 80; 		// 16Mhz / 80 = 200k

	//TRISE Configuration
	if(pI2CHandle->I2C_Config.u32_ClockSpeed <= I2C_SCL_SPEED_SM)
	{
		//Standard mode
		pI2CHandle->pI2Cx->I2C_TRISE.TRISE = (mcu_u32_ReadClkApb1() /1000000U) + 1 ;
	}
	else
	{
		//Fast mode
		pI2CHandle->pI2Cx->I2C_TRISE.TRISE = ( (mcu_u32_ReadClkApb1() * 300) / 1000000000U ) + 1;
	}
	
	// 
	pI2CHandle->pI2Cx->I2C_CR1.PE = 1; 			// Enable I2C
	
}

// Write 1-byte data through I2C 
static inline void mcu_u8_WriteI2cData( 	i2c_handle* pI2CHandle,
											uint8_t u8_DeviceAddr,
											uint8_t u8_SlaveAddr,
											uint8_t u8_SlaveValue)
{
	// 1. Master send write request to slave
	// 1.1 Generate START condition
	pI2CHandle->pI2Cx->I2C_CR1.START = 1;

	// 1.2 Confirm START condition by checking SB bit
	while( pI2CHandle->pI2Cx->I2C_SR1.SB == 0 );

	// 1.3 Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	// SlaveAddr = SlaveAddr << 1;
	// SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2CHandle->pI2Cx->I2C_DR.Register = (u8_DeviceAddr << 1) | 0; //Address is the slave address + r/nw bit=0

	// 4. Confirm address sent
	while( pI2CHandle->pI2Cx->I2C_SR1.ADDR == 0 );
	
	// 5. Clear the ADDR flag according to its software sequence
	mcu_ClearI2cAddrFlag(pI2CHandle);

	// 6. Send 1 byte data
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 ); // Wait till TXE is set
	pI2CHandle->pI2Cx->I2C_DR.DR = u8_SlaveAddr;
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 ); // Wait till TXE is set
	pI2CHandle->pI2Cx->I2C_DR.DR = u8_SlaveValue;

	// 7. Wait for TXE=1 and BTF=1 before the STOP condition
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 );
	while( pI2CHandle->pI2Cx->I2C_SR1.BTF == 0 );

	// 8. Check Acknowledge bit. If fail then stop and return
	pI2CHandle->pI2Cx->I2C_CR1.STOP = 1;		
}

// Read 1-byte data through I2C
static inline uint8_t mcu_u8_ReadI2cData( 		i2c_handle* pI2CHandle,
											uint8_t u8_DeviceAddr,
											uint8_t u8_SlaveAddr )
{
	uint8_t u8_SlaveValue;
	// 1. Master send write request to slave
	// 1.1 Generate START condition
	pI2CHandle->pI2Cx->I2C_CR1.START = 1;

	// 1.2 Confirm START condition by checking SB bit
	while( pI2CHandle->pI2Cx->I2C_SR1.SB == 0 );

	// 1.3 Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	// SlaveAddr = SlaveAddr << 1;
	// SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2CHandle->pI2Cx->I2C_DR.Register = (u8_DeviceAddr << 1) | 0; //Address is the slave address + r/nw bit=0

	// 4. Confirm address sent
	while( pI2CHandle->pI2Cx->I2C_SR1.ADDR == 0 );
	
	// 5. Clear the ADDR flag according to its software sequence
	mcu_ClearI2cAddrFlag(pI2CHandle);

	// 6. Send 1 byte data
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 ); // Wait till TXE is set
	pI2CHandle->pI2Cx->I2C_DR.DR = u8_SlaveAddr;

	// 7. Wait for TXE=1 and BTF=1 before the STOP condition
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 );
	while( pI2CHandle->pI2Cx->I2C_SR1.BTF == 0 );

	// 8. Check Acknowledge bit. If fail then stop and return
	if ( pI2CHandle->pI2Cx->I2C_SR1.AF == 1 )
	{
		pI2CHandle->pI2Cx->I2C_CR1.STOP = 1;
		return 0xFF;
	}
	
	// 2. Master get response from slave
	// 2.1 Generate START condition
	pI2CHandle->pI2Cx->I2C_CR1.START = 1;

	// 2.2 Confirm START condition by checking SB bit
	while( pI2CHandle->pI2Cx->I2C_SR1.SB == 0 );

	// 2.3 Send the address of the slave with r/nw bit set to 1 (total 8 bits )
	pI2CHandle->pI2Cx->I2C_DR.Register = (u8_DeviceAddr << 1) | 1; //Address is the slave address + r/nw bit=0

	// 2.4. Confirm address sent
	while( pI2CHandle->pI2Cx->I2C_SR1.ADDR == 0 );
	
	// 2.5 Clear the ADDR flag according to its software sequence
	mcu_ClearI2cAddrFlag(pI2CHandle);

	// 6. Send 1 byte data
	while( pI2CHandle->pI2Cx->I2C_SR1.RXNE == 0 ); // Wait till TXE is set
	u8_SlaveValue = pI2CHandle->pI2Cx->I2C_DR.DR;
	pI2CHandle->pI2Cx->I2C_SR1.ARLO = 1;

	// 7. Wait for TXE=1 and BTF=1 before the STOP condition
	return u8_SlaveValue;
	
		
}


// Send data through I2C
static inline void mcu_I2cMasterSendData( 	i2c_handle* pI2CHandle,
											const uint8_t *pTxbuffer, 
											uint32_t Len, 
											uint8_t SlaveAddr,
											uint8_t Sr )
{
	// 1. Generate START condition
	pI2CHandle->pI2Cx->I2C_CR1.START = 1;

	// 2. Confirm START condition by checking SB bit
	while( pI2CHandle->pI2Cx->I2C_SR1.SB == 0 );

	// 3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits )
	// SlaveAddr = SlaveAddr << 1;
	// SlaveAddr &= ~(1); //SlaveAddr is Slave address + r/nw bit=0
	pI2CHandle->pI2Cx->I2C_DR.Register = (SlaveAddr << 1) | 0; //Address is the slave address + r/nw bit=0

	// 4. Confirm address sent
	while( pI2CHandle->pI2Cx->I2C_SR1.ADDR == 0 );
	
	// 5. Clear the ADDR flag according to its software sequence
	mcu_ClearI2cAddrFlag(pI2CHandle);

	// 6. Send the data until len becomes 0
	for ( int i=0 ; i < Len; i++ )
	{
		while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 ); // Wait till TXE is set
		pI2CHandle->pI2Cx->I2C_DR.DR = pTxbuffer[i];
	}

	// 7. Wait for TXE=1 and BTF=1 before the STOP condition
	while( pI2CHandle->pI2Cx->I2C_SR1.TXE == 0 );
	while( pI2CHandle->pI2Cx->I2C_SR1.BTF == 0 );

	// 8. Generate STOP condition. Don't need to wait for stop condition finish in master mode.
	if( Sr == 0 )
		pI2CHandle->pI2Cx->I2C_CR1.STOP = 1;
}

static void mcu_ClearI2cAddrFlag(i2c_handle *pI2CHandle )
{
	uint32_t dummy_read;

	if ( pI2CHandle->pI2Cx->I2C_SR2.MSL == 1 )
	{
		// Master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize  == 1)
			{
				// Disable ack
				pI2CHandle->pI2Cx->I2C_CR1.ACK = 0;

				//clear the ADDR flag by reading SR1 and SR2
				dummy_read = pI2CHandle->pI2Cx->I2C_SR1.Register;
				dummy_read = pI2CHandle->pI2Cx->I2C_SR2.Register;
				(void)dummy_read;
			}

		}
		else
		{
			//clear the ADDR flag by reading SR1 and SR2
			dummy_read = pI2CHandle->pI2Cx->I2C_SR1.Register;
			dummy_read = pI2CHandle->pI2Cx->I2C_SR2.Register;
			(void)dummy_read;
		}

	}
	else
	{
		// Slave mode
		//clear the ADDR flag by reading SR1 and SR2
		dummy_read = pI2CHandle->pI2Cx->I2C_SR1.Register;
		dummy_read = pI2CHandle->pI2Cx->I2C_SR2.Register;
		(void)dummy_read;
	}
}


// Receive data through I2C
static inline void mcu_I2cMasterReceiveData( i2c_handle* pI2CHandle, uint8_t *pTxbuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr )
{
}

static void mcu_InitI2c1( uint8_t address )
{
	i2c1_handle.pI2Cx 						= I2C1;
	i2c1_handle.I2C_Config.u32_ClockSpeed 	= I2C_SCL_SPEED_SM;
	i2c1_handle.I2C_Config.u8_Address 		= address;
	i2c1_handle.I2C_Config.u8_AckControl 	= 1;
	i2c1_handle.I2C_Config.u8_DutyCycle 	= 2;
	mcu_I2cInit(&i2c1_handle);
	mcu_SetGpioAlternate("B6", 4); 		// IC1 SCL.
	mcu_SetGpioAlternate("B9", 4); 		// IC1 SDA.
}

static void mcu_I2c1SendData( const uint8_t *pTxBuffer, uint8_t SlaveAddr)
{
	mcu_I2cMasterSendData( &i2c1_handle, pTxBuffer, 10, SlaveAddr, 1 );
}

static void mcu_WriteI2c1( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_SlaveValue )
{	
	return mcu_u8_WriteI2cData( &i2c1_handle, u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
}


static uint8_t mcu_u8_ReadI2c1( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr )
{	
	return mcu_u8_ReadI2cData( &i2c1_handle, u8_DeviceAddress, u8_SlaveAddr);
}

static void mcu_I2c1InitBuffer( uint8_t *pBuffer)
{
	i2c1_buffer = pBuffer;
}

static void mcu_I2c1IrqService(void)
{
}


static void mcu_InitI2c2( uint8_t address )
{
	i2c2_handle.pI2Cx 						= I2C2;
	i2c2_handle.I2C_Config.u32_ClockSpeed 	= I2C_SCL_SPEED_SM;
	i2c2_handle.I2C_Config.u8_Address 		= address;
	i2c2_handle.I2C_Config.u8_AckControl 	= 1;
	i2c2_handle.I2C_Config.u8_DutyCycle 	= 2;
	mcu_I2cInit(&i2c2_handle);
	mcu_SetGpioAlternate("B10", 4); 		// IC2 SCL.
	mcu_SetGpioAlternate("B11", 4); 		// IC2 SDA.
}

static void mcu_I2c2SendData(const uint8_t *pTxBuffer, uint8_t SlaveAddr)
{
	mcu_I2cMasterSendData( &i2c2_handle, pTxBuffer, 10, SlaveAddr, 1 );
}

static void mcu_WriteI2c2( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_SlaveValue )
{	
	return mcu_u8_WriteI2cData( &i2c2_handle, u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
}


static uint8_t mcu_u8_ReadI2c2( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr )
{	
	return mcu_u8_ReadI2cData( &i2c2_handle, u8_DeviceAddress, u8_SlaveAddr);
}

static void mcu_I2c2InitBuffer(uint8_t *pBuffer)
{
	i2c2_buffer = pBuffer;
}

static void mcu_I2c2IrqService(void)
{
}

static void mcu_InitI2c3( uint8_t address )
{
	i2c3_handle.pI2Cx 						= I2C3;
	i2c3_handle.I2C_Config.u32_ClockSpeed 	= I2C_SCL_SPEED_SM;
	i2c3_handle.I2C_Config.u8_Address 		= address;
	i2c3_handle.I2C_Config.u8_AckControl 	= 1;
	i2c3_handle.I2C_Config.u8_DutyCycle 	= 2;
	mcu_I2cInit(&i2c3_handle);
	mcu_SetGpioAlternate("A8", 4); 		// IC3 SCL.
	mcu_SetGpioAlternate("C9", 4); 		// IC3 SDA.
}

static void mcu_WriteI2c3( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr, uint8_t u8_SlaveValue )
{	
	return mcu_u8_WriteI2cData( &i2c3_handle, u8_DeviceAddress, u8_SlaveAddr, u8_SlaveValue);
}


static uint8_t mcu_u8_ReadI2c3( uint8_t u8_DeviceAddress, uint8_t u8_SlaveAddr )
{	
	return mcu_u8_ReadI2cData( &i2c3_handle, u8_DeviceAddress, u8_SlaveAddr);
}


static void mcu_I2c3SendData(const uint8_t *pTxBuffer, uint8_t SlaveAddr)
{
	mcu_I2cMasterSendData( &i2c3_handle, pTxBuffer, 10, SlaveAddr, 1 );
}

static void mcu_I2c3InitBuffer(uint8_t *pBuffer)
{
	i2c3_buffer = pBuffer;
}

static void mcu_I2c3IrqService(void)
{
}

/********************************************************************************
 * 									ARM RESET									*
 * 								Software reset									*
 * ******************************************************************************/
static inline void mcu_SoftReset(void)
{
	__DSB();
	SCB->AIRCR.Register  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR.Register & (0x700)) | (1<<NVIC_SYSRESETREQ));  	// Keep priority group unchanged  
	/*
	// Don't know why, need to write the whole register together to make the reset work instead of programming every fields
	SCB->AIRCR.VECTKEY = 0x5FA;
	SCB->AIRCR.VECTRESET = 0x0;
	SCB->AIRCR.VECTCLRACTIVE = 0x0;  
	SCB->AIRCR.SYSRESETREQ = 0x1;		//Assert signal request reset
	*/
	
	__DSB();
	while(1);                                                             	// wait until reset
}