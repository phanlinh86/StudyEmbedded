//
// Created by Phan Linh on 25/12/2024.
//
static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );
static inline void mcu_SetGpioInterrupt( char* pinString );

/********************************************************************************
*									STM32 GPIOA_BASE							*
*									General Purpose I/O							*
*********************************************************************************/

static inline void mcu_SetGpioOutput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	RCC->RCC_IOPENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	// GPIO->MODER.Register |= ( 1 << ( pin * 2 ) );			// Set GPIO mode to output
	BITFIELDMNSET(GPIO->MODER.Register, pin*2+1, pin*2, 1);
}

static inline void mcu_SetGpioInput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
  	RCC->RCC_IOPENR.Register |= ( 1 << ( port - 'A' ) ); 	// Enable GPIO clock
	// GPIO->MODER.Register &= ~( 1 << ( pin * 2 ) );			// Set GPIO mode to output
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

static inline void mcu_SetGpioInterrupt( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	gpio_regs *GPIO = (gpio_regs*)( GPIOA_BASE + ( port - 'A' ) * 0x400 );
	// Mux PA0 to PA0
	SYSCFG->SYSCFG_EXTICR1.EXTI0 = 0;
	// Set rising edge detection
	EXTI->EXTI_RTSR.TR0 = 1;
	// Disable EXTI0 Interrupt Masking
	EXTI->EXTI_IMR.MR0 = 1;
	// APB2 SYSCFG Clock Enable
	RCC->RCC_APB2ENR |= 1 << 14;
	// Enable NVIC
	*(NVIC_ISER0) |= 1 << 6;
}