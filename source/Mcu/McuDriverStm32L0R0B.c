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

static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );
static inline void mcu_Usart_Init(uint32_t baudRate);


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
	
	if ( u16_AhbPreScale > 8 )
		u16_AhbPreScale = 1 << (u16_AhbPreScale - 7);
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