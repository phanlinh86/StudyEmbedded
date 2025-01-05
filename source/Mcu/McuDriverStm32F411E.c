//
// Created by Phan Linh on 25/12/2024.
//
#pragma once

typedef enum 
{
	USART_NO_STOP_BITS_0p5 	= 0,
	USART_NO_STOP_BITS_1p0	= 1,
	USART_NO_STOP_BITS_1p5 	= 2,
	USART_NO_STOP_BITS_2p0 	= 3,
} usart_nostopbit;

typedef enum
{
	USART_MODE_ONLY_TX 		= 0,
	USART_MODE_ONLY_RX 		= 1,
	USART_MODE_TXRX  		= 2,
} usart_mode;

typedef enum
{
	USART_PARITY_DISABLE 	= 0,
	USART_PARITY_EN_ODD 	= 1,
	USART_PARITY_EN_EVEN 	= 2,
} usart_parity_mode;

typedef enum
{
	RCC_CLK_SRC_HSI 		= 0,
	RCC_CLK_SRC_HSE 		= 1,
	RCC_CLK_SRC_PLL 		= 2,
} rcc_clk_src;


typedef struct 
{
	usart_mode eMode;
	usart_nostopbit eNoOfStopBits;
	usart_parity_mode eParityControl;
	uint32_t u32_BaudRate;
	uint8_t u8_WordLength;
	// uint8_t u8_HWFlowControl;
} usart_config;

typedef struct
{
	usart_regs 		*pUSARTx; 			// Pointer to go to USART register set
	usart_config   	USART_Config;		// Struct to store the config
}usart_handle;

static usart_handle usart1_handle;
static usart_handle usart2_handle;


static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );
static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue );

// USART2
static void mcu_InitUsart2();
static void mcu_Usart2SendData(const char *pTxBuffer);
// USART1
static void mcu_InitUsart1();
static void mcu_Usart1SendData(const char *pTxBuffer);
// USART6
static inline void mcu_UsartInit( usart_handle* pUsartHandle ); 		// Enable and initiliaze USART
static inline void mcu_UsartSetBaudRate( usart_handle *pUSARTHandle );	// Set USART baud rate
void mcu_UsartSendData(usart_handle *pUSARTHandle, const char *pTxBuffer);
void mcu_UsartSendChar(usart_handle *pUSARTHandle, char data);
void mcu_UsartReceiveData(usart_handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
static uint32_t mcu_ReadClkApb1();
static uint32_t mcu_ReadClkApb2();


/********************************************************************************
*									STM32 GPIOA_BASE							*
*									General Purpose I/O							*
*********************************************************************************/

static inline void mcu_SetGpioOutput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	RCC->RCC_AHB1ENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 1);
}

static inline void mcu_SetGpioInput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
  	RCC->RCC_AHB1ENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 0);
}

static inline void mcu_ToggleGpio( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);	
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register ^= ( 1 << pin );			// Set GPIO mode to output
}

static inline void mcu_SetGpioHigh( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register |= ( 1 << pin );			// Set GPIO mode to output
}

static inline void mcu_SetGpioLow( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	GPIO->ODR.Register &= ~( 1 << pin );			// Set GPIO mode to output
}

static inline void mcu_SetGpioAlternate( char* pinString, uint8_t u8_AlternateValue )
{
	char port; int pin;
	
	sscanf( pinString, "%c%d", &port, &pin);
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
	usart1_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_115200;
	usart1_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart1_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart1_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart1_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart1_handle);
	mcu_SetGpioAlternate("B6", 7); 	// USART1 TX. Use B6 instead of A9 due to A9 connected to a LED with capacitor
	mcu_SetGpioAlternate("B7", 7); 	// USART1 RX. Use B6 instead of A9 due to A10 connected to a LED with capacitor
}

static void mcu_Usart1SendData(const char *pTxBuffer)
{
	mcu_UsartSendData( &usart1_handle, pTxBuffer );
}
 
void mcu_InitUsart2(void)
{	
	usart2_handle.pUSARTx 						= USART2;
	usart2_handle.USART_Config.u32_BaudRate 	= USART_BAUDRATE_115200;
	usart2_handle.USART_Config.eMode 			= USART_MODE_TXRX;
	usart2_handle.USART_Config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart2_handle.USART_Config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart2_handle.USART_Config.eParityControl 	= USART_PARITY_DISABLE;
	mcu_UsartInit(&usart2_handle);
	mcu_SetGpioAlternate("A2", 7); 	// USART2 TX
	mcu_SetGpioAlternate("A3", 7); 	// USART3 RX	
}

static void mcu_Usart2SendData(const char *pTxBuffer)
{
	mcu_UsartSendData( &usart2_handle, pTxBuffer );
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
	}else if (pUSARTHandle->USART_Config.eMode == USART_MODE_ONLY_TX)
	{
		//Implement the code to enable the Transmitter bit field 
		pUSARTHandle->pUSARTx->USART_CR1.TE = 1;

	}else if (pUSARTHandle->USART_Config.eMode == USART_MODE_TXRX)
	{
		//Implement the code to enable the both Transmitter and Receiver bit fields 
		pUSARTHandle->pUSARTx->USART_CR1.TE = 1;
		pUSARTHandle->pUSARTx->USART_CR1.RE = 1;
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
		u32_ApbClk = mcu_ReadClkApb2();
	}
	else if (pUSARTHandle->pUSARTx == USART2)
	{
		u32_ApbClk = mcu_ReadClkApb1();
	}
	OVER8 = pUSARTHandle->pUSARTx->USART_CR1.OVER8;
	// Formula from the specs
	// Tx/Rx baud = u32_ApbClk / ( 8 * (2-OVER8) * USARTDIV );
	// USARTDIV 	= u32_ApbClk / ( 8 * (2-OVER8) ) / Tx/Rx baud;
	USARTDIV = ( u32_ApbClk * 16 ) / ( 8 * (2-OVER8) ) / pUSARTHandle->USART_Config.u32_BaudRate;
	pUSARTHandle->pUSARTx->USART_BRR.Register = USARTDIV;

}

void mcu_UsartSendChar(usart_handle *pUSARTHandle, char data)
{
	while(!pUSARTHandle->pUSARTx->USART_SR.TXE);
	
	if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		pUSARTHandle->pUSARTx->USART_DR.DR = data & (uint16_t)0x01FF;
	else
		pUSARTHandle->pUSARTx->USART_DR.DR = data & (uint8_t)0xFF;
}


void mcu_UsartSendData(usart_handle *pUSARTHandle, const char *pTxBuffer)
{
	uint16_t index=0;

	while  ( pTxBuffer[index] )
	{
		mcu_UsartSendChar(pUSARTHandle, pTxBuffer[index]);
		if ( (pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS) && 
			 (pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE) )
			index += 2;
		else
			index++;
	}
	//Implement the code to wait till TC flag is set in the SR
	while(!pUSARTHandle->pUSARTx->USART_SR.TC);
}

void mcu_UsartReceiveData(usart_handle *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{

   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//Implement the code to wait until RXNE flag is set in the SR
		while(!pUSARTHandle->pUSARTx->USART_SR.RXNE);

		//Check the USART_WordLength to decide whether we are going to receive 9bit of data in a frame or 8 bit
		if(pUSARTHandle->USART_Config.u8_WordLength == USART_WORDLEN_9BITS)
		{
			//We are going to receive 9bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 9bits will be of user data
				//read only first 9 bits so mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR.DR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR.DR  & (uint8_t)0xFF);
				 pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//Now, check are we using USART_ParityControl control or not
			if(pUSARTHandle->USART_Config.eParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data
				//read 8 bits from DR
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR.DR  & (uint8_t)0xFF);
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity
				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR.DR  & (uint8_t)0x7F);
			}

			//Now , increment the pRxBuffer
			pRxBuffer++;
		}
	}

}



/********************************************************************************
 * 									STM32 RCC									*
 * 						Reset and Clock Control									*
 * ******************************************************************************/
 
static uint32_t mcu_ReadClkApb1()
{
	unsigned u16_Apb1PreScale;
	unsigned u16_AhbPreScale;
	uint32_t u32_SysClk;
	rcc_clk_src eRccClkSrc; 
	eRccClkSrc = RCC->RCC_CFGR.SWS;
	switch (eRccClkSrc)
	{
		case RCC_CLK_SRC_HSI:
			u32_SysClk = 16000000; 	// HSI using 16Mhz clock
			break;
		case RCC_CLK_SRC_HSE:
			u32_SysClk = 16000000; 	// HSI using 16Mhz clock
			break;
		case RCC_CLK_SRC_PLL:
			u32_SysClk = 48000000; 	// PLL using 48Mhz clock
			break;
	}
	u16_AhbPreScale = RCC->RCC_CFGR.HPRE;
	if ( u16_AhbPreScale > 4 )
		u16_AhbPreScale = 1 << (u16_AhbPreScale - 3);
	else
		u16_AhbPreScale = 1;
	
	u16_Apb1PreScale = RCC->RCC_CFGR.PPRE1;
	if ( u16_Apb1PreScale > 4 )
		u16_Apb1PreScale = 1 << (u16_Apb1PreScale - 3);
	else
		u16_Apb1PreScale = 1;
	return (u32_SysClk / u16_AhbPreScale)  / u16_Apb1PreScale;
	
}

static uint32_t mcu_ReadClkApb2()
{
	unsigned u16_Apb2PreScale;
	unsigned u16_AhbPreScale;
	uint32_t u32_SysClk;
	rcc_clk_src eRccClkSrc; 
	eRccClkSrc = RCC->RCC_CFGR.SWS;
	switch (eRccClkSrc)
	{
		case RCC_CLK_SRC_HSI:
			u32_SysClk = 16000000; 	// HSI using 16Mhz clock
			break;
		case RCC_CLK_SRC_HSE:
			u32_SysClk = 16000000; 	// HSI using 16Mhz clock
			break;
		case RCC_CLK_SRC_PLL:
			u32_SysClk = 48000000; 	// PLL using 48Mhz clock
			break;
	}
	u16_AhbPreScale = RCC->RCC_CFGR.HPRE;
	if ( u16_AhbPreScale > 4 )
		u16_AhbPreScale = 1 << (u16_AhbPreScale - 3);
	else
		u16_AhbPreScale = 1;
	u16_Apb2PreScale = RCC->RCC_CFGR.PPRE2;
	if ( u16_Apb2PreScale > 4 )
		u16_Apb2PreScale = 1 << (u16_Apb2PreScale - 3);
	else
		u16_Apb2PreScale = 1;
	return (u32_SysClk / u16_AhbPreScale)  / u16_Apb2PreScale;
	
}