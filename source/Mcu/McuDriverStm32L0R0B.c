//
// Created by Phan Linh on 25/12/2024.
//

typedef enum
{
	RCC_CLK_SRC_MSI			= 0,
	RCC_CLK_SRC_HSI 		= 1,
	RCC_CLK_SRC_HSE 		= 2,
	RCC_CLK_SRC_PLL 		= 3,
} rcc_clk_src;

typedef struct
{
	usart_regs 		*pUSARTx; 			// Pointer to go to USART register set
	usart_config   	USART_Config;		// Struct to store the config
}usart_handle;
static usart_handle usart2_handle;
static uint8_t* usart2_buffer;
static uint8_t usart2_buffer_len = 0;
static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline uint8_t mcu_u8_ReadGpio( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );
static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue );



// USART2
static void mcu_InitUsart2();
static void mcu_Usart2SendData(const char *pTxBuffer);
static void mcu_Usart2InitBuffer(char *pBuffer);
static void mcu_Usart2IrqService(void);
static usart_config mcu_GetUsart2Config(void);

static inline void mcu_UsartInit( usart_handle* pUsartHandle ); 		// Enable and initiliaze USART
static inline void mcu_UsartSetBaudRate( usart_handle *pUSARTHandle );	// Set USART baud rate
void mcu_UsartSendData(usart_handle *pUSARTHandle, const char *pTxBuffer);
void mcu_UsartSendChar(usart_handle *pUSARTHandle, char data);
static char mcu_UsartDataAvailable(usart_handle *pUSARTHandle);
void mcu_UsartReceiveData(usart_handle *pUSARTHandle, char *str);
uint8_t mcu_UsartGetChar(usart_handle *pUSARTHandle);

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
	RCC->RCC_IOPENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	// GPIO->MODER.Register |= ( 1 << ( pin * 2 ) );			// Set GPIO mode to output
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 1);
}

static inline void mcu_SetGpioInput( char* pinString )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
  	RCC->RCC_IOPENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	// GPIO->MODER.Register &= ~( 1 << ( pin * 2 ) );			// Set GPIO mode to output
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
	return ( ( GPIO->ODR.Register >> pin ) & 0x01 );			// Set GPIO mode to output
}

static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue )
{
	char port; uint8_t pin;
	ut_GetPortAndPinFromString( pinString, &port, &pin );
	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	RCC->RCC_IOPENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock		
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 2); 	// Set moder to 2 for alternate function
	BITFIELDMNSET(GPIO->PUPDR.Register, pin*2+1, pin*2, 1);		// Set pull up register for correct start bit
	GPIO->OTYPER &= ~ (1 << pin);		// Set Otyper
	BITFIELDMNSET(GPIO->OSPEEDR, pin*2+1, pin*2, 2);
	
	if ( pin < 8 )
		BITFIELDMNSET(GPIO->AFRL.Register, pin*4+3, pin*4, u8_AlternateValue);	// Set Alternate function
	else
		BITFIELDMNSET(GPIO->AFRH.Register, (pin-8)*4+3, (pin-8)*4, u8_AlternateValue);	// Set Alternate function
}


/********************************************************************************
 * 									STM32 USART_BASE							*
 * 						Universal Synchronous Asynchronous Receiver Transmitter	*
 * ******************************************************************************/
void mcu_InitUsart2(void)
{	
	usart2_handle.pUSARTx 						= USART2;
	usart2_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_115200;
	usart2_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart2_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart2_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart2_handle);
	mcu_SetGpioAlternate("A2", 4); 	// USART2 TX
	mcu_SetGpioAlternate("A3", 4); 	// USART2 RX	
}

static void mcu_Usart2SendData(const char *pTxBuffer)
{
	mcu_UsartSendData( &usart2_handle, pTxBuffer );
}

static void mcu_Usart2InitBuffer(char *pBuffer)
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

void mcu_UsartInit(usart_handle *pUSARTHandle)
{

	//Temporary variable
	uint32_t u32_TempReg = 0;

	/******************************** Configuration of CR1******************************************/

	//Implement the code to enable the Clock for given USART peripheral
	
	RCC->RCC_APB1ENR.USART2EN = 1;

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
	pUSARTHandle->pUSARTx->USART_CR1.M0 = pUSARTHandle->USART_Config.u8_WordLength - 8;


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
	u32_ApbClk = mcu_u32_ReadClkApb1();
	OVER8 = pUSARTHandle->pUSARTx->USART_CR1.OVER8;
	// Formula from the specs
	// Tx/Rx baud = u32_ApbClk / ( 8 * (2-OVER8) * USARTDIV );
	// USARTDIV 	= u32_ApbClk / ( 8 * (2-OVER8) ) / Tx/Rx baud;
	USARTDIV = ( u32_ApbClk  * 16 ) / ( 8 * (2-OVER8) ) / pUSARTHandle->USART_Config.u32_BaudRate;
	pUSARTHandle->pUSARTx->USART_BRR.Register = USARTDIV;

}

void mcu_UsartSendChar(usart_handle *pUSARTHandle, char data)
{
	while(!pUSARTHandle->pUSARTx->USART_ISR.TXE);
	
	if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		pUSARTHandle->pUSARTx->USART_TDR.DR = data & (uint16_t)0x01FF;
	else
		pUSARTHandle->pUSARTx->USART_TDR.DR = data & (uint8_t)0xFF;
}

char mcu_UsartDataAvailable(usart_handle *pUSARTHandle)
{
	return pUSARTHandle->pUSARTx->USART_ISR.RXNE;
}

uint8_t mcu_UsartGetChar(usart_handle *pUSARTHandle)
{
    // while(!pUSARTHandle->pUSARTx->USART_ISR.RXNE); 	// Since this will be handled by interrupt
    if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			return (char)(pUSARTHandle->pUSARTx->USART_RDR.DR & (uint16_t)0x01FF);
		else
			return (char)(pUSARTHandle->pUSARTx->USART_RDR.DR & (uint8_t)0x0FF);
    else
		if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			return (char)(pUSARTHandle->pUSARTx->USART_RDR.DR & (uint8_t)0xFF);
		else
			return (char)(pUSARTHandle->pUSARTx->USART_RDR.DR  & (uint8_t)0x7F);
}

void mcu_UsartSendData(usart_handle *pUSARTHandle, const char *pTxBuffer)
{
	static uint8_t j=0;
	
	for (j=0; j<= TX_USART_BUFFER-1; j++)
		mcu_UsartSendChar(pUSARTHandle, pTxBuffer[j]);

	//Implement the code to wait till TC flag is set in the SR
	while(!pUSARTHandle->pUSARTx->USART_ISR.TC);
}


void mcu_UsartReceiveData(usart_handle *pUSARTHandle, char *str)
{
    unsigned char data;
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

static usart_config mcu_GetUsart2Config(void)
{
	return usart2_handle.USART_Config;
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
		case RCC_CLK_SRC_MSI:
			return 	2097000; 	// MSI using 16Mhz clock
		case RCC_CLK_SRC_HSI:
			return 16000000; 	// HSI using 16Mhz clock
		case RCC_CLK_SRC_HSE:
			return 	4000000; 	// HSI using 16Mhz clock
		case RCC_CLK_SRC_PLL:
			return 48000000; 	// PLL using 48Mhz clock
	}
}

static uint32_t mcu_u32_ReadAhbClk()
{
	uint32_t u32_SysClk = mcu_u32_ReadSysClk();
	unsigned u16_AhbPreScale = RCC->RCC_CFGR.HPRE;
	
	if ( u16_AhbPreScale > 7 )
		u16_AhbPreScale = 1 << (u16_AhbPreScale - 7);
	else
		u16_AhbPreScale = 1;
	
	return u32_SysClk / u16_AhbPreScale;
}

static uint32_t mcu_u32_ReadClkApb1()
{
	unsigned u16_Apb1PreScale = RCC->RCC_CFGR.PPRE1;
	uint32_t u32_AhbClk = mcu_u32_ReadAhbClk();

	if ( u16_Apb1PreScale > 3 )
		u16_Apb1PreScale = 1 << (u16_Apb1PreScale - 3);
	else
		u16_Apb1PreScale = 1;
	
	return u32_AhbClk  / u16_Apb1PreScale;
	
}

static uint32_t mcu_u32_ReadClkApb2()
{
	unsigned u16_Apb2PreScale = RCC->RCC_CFGR.PPRE2;
	uint32_t u32_AhbClk = mcu_u32_ReadAhbClk();

	if ( u16_Apb2PreScale > 3 )
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
 * 									ARM RESET									*
 * 								Software reset									*
 * ******************************************************************************/
static inline void mcu_SoftReset(void)
{
	//  Place holder. No support. Do nothing
}