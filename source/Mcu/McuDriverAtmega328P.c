#pragma once

#define USART_BAUDRATE_9600 9600
#define USART_BAUDRATE_19200 19200
#define USART_BAUDRATE_38400 38400
#define USART_BAUDRATE_57600 57600
#define USART_BAUDRATE_115200 115200
#define USART_BAUDRATE_230400 230400
#define USART_BAUDRATE_460800 460800
#define USART_BAUDRATE_921600 921600
#define USART_BAUDRATE_2250000 2000000
#define USART_BAUDRATE_4500000 3000000
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

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

typedef struct 
{
	usart_mode eMode;
	usart_nostopbit eNoOfStopBits;
	usart_parity_mode eParityControl;
	uint32_t u32_BaudRate;
	uint8_t u8_WordLength;
} usart_config;


static usart_config usart0_config;
static char* usart0_buffer;
static char usart0_buffer_len = 0;

static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );


static inline uint32_t mcu_u32_ReadSrcClk();
static inline uint32_t mcu_u32_ReadSystemClk();

static uint32_t mcu_SetTimer1Clk(uint32_t u32_Timer1PeriodInUs);
static void mcu_ResetTimer1(void);
static void mcu_EnableTimer1Interrupt(void);

// REG
static void mcu_InitUsart0();
static void mcu_Usart0SendData(const char *pTxBuffer);
static void mcu_Usart0InitBuffer(char *pBuffer);
static void mcu_Usart0IrqService(void);

// General USART
static void mcu_UsartTransmit(char data);
static uint8_t mcu_u8_UsartReceive(void);
static uint8_t mcu_u8_UsartDataAvailable ( void );
static void mcu_UsartTransmitString(const char *str);
// static void mcu_UsartReceiveString(char *str);
static void mcu_UsartInit(usart_config usartx_config);
static inline void mcu_UsartSetBaudRate( uint32_t u32_UartBaudRate);


/********************************************************************************
*									ATMega328P GPIO_BASE						*
*									General Purpose I/O							*
*********************************************************************************/

static inline void mcu_SetGpioOutput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	switch (port)
	{
		case 'B':
			REG->DDRB |= 1 << pin;
			break;
		case 'C':
			REG->DDRC |= 1 << pin;
			break;
		case 'D':
			REG->DDRD |= 1 << pin;
			break;
	}
	
}

static inline void mcu_SetGpioInput( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);	
	switch (port)
	{
		case 'B':
			REG->DDRB &= ~(1 << pin);
			break;
		case 'C':
			REG->DDRC &= ~(1 << pin);
			break;
		case 'D':
			REG->DDRD &= ~(1 << pin);
			break;
	}
}

static inline void mcu_ToggleGpio( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);	
	switch (port)
	{
		case 'B':
			REG->PORTB ^= 1 << pin;
			break;
		case 'C':
			REG->PORTB ^= 1 << pin;
			break;
		case 'D':
			REG->PORTD ^= 1 << pin;
			break;
	}
}

static inline void mcu_SetGpioHigh( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	switch (port)
	{
		case 'B':
			REG->PORTB |= 1 << pin;
			break;
		case 'C':
			REG->PORTB |= 1 << pin;
			break;
		case 'D':
			REG->PORTD |= 1 << pin;
			break;
	}
}

static inline void mcu_SetGpioLow( char* pinString )
{
	char port; int pin;
	sscanf( pinString, "%c%d", &port, &pin);
	switch (port)
	{
		case 'B':
			REG->PORTB &= ~(1 << pin);
			break;
		case 'C':
			REG->PORTB &= ~(1 << pin);
			break;
		case 'D':
			REG->PORTD &= ~(1 << pin);
			break;
	}
}



/********************************************************************************
*									ATMega328P System Clock						*
*									and Clock options							*
*********************************************************************************/
static inline uint32_t mcu_u32_ReadSrcClk()
{
	return F_CPU;
}

static inline uint32_t mcu_u32_ReadSystemClk()
{
	uint16_t u16_SystemPreScaler = 0;
	u16_SystemPreScaler = 	REG->CLKPR.CLKPS0 
							+ (REG->CLKPR.CLKPS1 << 1)
							+ (REG->CLKPR.CLKPS2 << 2)
							+ (REG->CLKPR.CLKPS3 << 3);		
	if ( u16_SystemPreScaler > 8 )
		u16_SystemPreScaler = 1;	// Invalid setting. Set to 0.
		
	u16_SystemPreScaler = 1 << u16_SystemPreScaler;
	return mcu_u32_ReadSrcClk() / u16_SystemPreScaler;			
}

static uint32_t mcu_SetTimer1Clk(uint32_t u32_Timer1PeriodInUs)
{

    /*
              MAX x Prescale
        Ts = ---------------
              F_CPU x 10^6
    */

    uint32_t cycles = (mcu_u32_ReadSystemClk() / 2000000) * u32_Timer1PeriodInUs; // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2   

    if	(cycles < TIMER1_RESOLUTION)              
	{
		// prescale by /1
		REG->TCCR1B.CS12 = 0;
		REG->TCCR1B.CS11 = 0;
		REG->TCCR1B.CS10 = 1;
	}
    else if((cycles >>= 3) < TIMER1_RESOLUTION) 
	{
		// prescale by /8
		REG->TCCR1B.CS12 = 0;
		REG->TCCR1B.CS11 = 1;
		REG->TCCR1B.CS10 = 0;		
	}
    else if((cycles >>= 3) < TIMER1_RESOLUTION)
	{
		// prescale by /64
		REG->TCCR1B.CS12 = 0;
		REG->TCCR1B.CS11 = 1;
		REG->TCCR1B.CS10 = 1;		
	}					
    else if((cycles >>= 2) < TIMER1_RESOLUTION) 
	{
		// prescale by /256
		REG->TCCR1B.CS12 = 1;
		REG->TCCR1B.CS11 = 0;
		REG->TCCR1B.CS10 = 0;				
	}
    else if((cycles >>= 2) < TIMER1_RESOLUTION)
	{
		// prescale by /1024
		REG->TCCR1B.CS12 = 1;
		REG->TCCR1B.CS11 = 0;
		REG->TCCR1B.CS10 = 1;				
	}		
    else
	{        
		// request was out of bounds
		cycles = TIMER1_RESOLUTION - 1;
		REG->TCCR1B.CS12 = 1;
		REG->TCCR1B.CS11 = 0;
		REG->TCCR1B.CS10 = 1;
	}

    REG->TCNT1.Register = 0; // Reset the counter
	REG->TCCR1B.CS12 = 0;
	REG->TCCR1B.CS11 = 0;
	REG->TCCR1B.CS10 = 1;	
	return cycles;
}

static void mcu_ResetTimer1(void)
{
	REG->TCCR1A.Register = 0;					// Default Timer1 Control. No PWM, no Compare mode.
    REG->TCCR1B.Register = 0;					// Default Timer1 Control. Timer1 Stop
    REG->TIMSK1.Register = 0;					// No interrupt
}

static void mcu_EnableTimer1Interrupt(void)
{
	REG->TIMSK1.TOIE = 1;                  	// Overflow interrupt enable
}


/********************************************************************************
*									ATMega328P System Clock						*
*									and Clock options							*
*********************************************************************************/
static void mcu_InitUsart0(void)
{
	usart0_config.u32_BaudRate 		= USART_BAUDRATE_9600;
	usart0_config.eMode 			= USART_MODE_TXRX;
	usart0_config.eNoOfStopBits 	= USART_NO_STOP_BITS_1p0;
	usart0_config.u8_WordLength 	= USART_WORDLEN_8BITS;
	usart0_config.eParityControl 	= USART_PARITY_DISABLE;
	
	mcu_UsartInit( usart0_config );
}

static void mcu_UsartInit(usart_config usartx_config)
{
	// Reset USART control registers before setting bits
	REG->UCSR0C.Register = 0;
	REG->UCSR0B.Register = 0;

	// Set baud rate
	mcu_UsartSetBaudRate(usartx_config.u32_BaudRate);
	
	// Word Length
	switch ( usartx_config.u8_WordLength )
	{
		case USART_WORDLEN_8BITS:
			REG->UCSR0B.UCSZ02 = 0;
			REG->UCSR0C.UCSZ01 = 1;
			REG->UCSR0C.UCSZ00 = 1;
			break;
		case USART_WORDLEN_9BITS:
			REG->UCSR0B.UCSZ02 = 1;
			REG->UCSR0C.UCSZ01 = 1;
			REG->UCSR0C.UCSZ00 = 1;
			break;
	}
	// Parity
	switch ( usartx_config.eParityControl )
	{
		case USART_PARITY_DISABLE:
			REG->UCSR0C.UPM01 = 0;
			REG->UCSR0C.UPM00 = 0;
			break;
		case USART_PARITY_EN_ODD:
			REG->UCSR0C.UPM01 = 1;
			REG->UCSR0C.UPM00 = 0;
			break;
		case USART_PARITY_EN_EVEN:
			REG->UCSR0C.UPM01 = 1;
			REG->UCSR0C.UPM00 = 1;
			break;
	}
	
	// Stop bit
	switch ( usartx_config.eNoOfStopBits )
	{
		case USART_NO_STOP_BITS_0p5:
			break;
		case USART_NO_STOP_BITS_1p0:
			REG->UCSR0C.USBS0 = 0;
			break;
		case USART_NO_STOP_BITS_1p5:
			break;
		case USART_NO_STOP_BITS_2p0:
			REG->UCSR0C.USBS0 = 1;
			break;
	}
	
	// Asynchrounous
	REG->UCSR0C.UMSEL01 = 0;
	REG->UCSR0C.UMSEL00 = 0;
	
	REG->UCSR0C.UCPOL0 = 0;		// Transmit: Rising Edge.  Receive: Falling Edge
	
	// Mode
	switch (usartx_config.eMode)
	{
		case USART_MODE_ONLY_RX:
			REG->UCSR0B.RXEN0 = 1;
			REG->UCSR0B.TXEN0 = 0;
			REG->UCSR0B.RXCIE0 = 1;			// Enable receiver interrupt
			break;
		case USART_MODE_ONLY_TX:
			REG->UCSR0B.RXEN0 = 0;
			REG->UCSR0B.TXEN0 = 1;
			REG->UCSR0B.RXCIE0 = 0;			// Disable receiver interrupt
			break;
		case USART_MODE_TXRX:
			REG->UCSR0B.RXEN0 = 1;
			REG->UCSR0B.TXEN0 = 1;
			REG->UCSR0B.RXCIE0 = 0;			// Enable receiver interrupt
			break;
	}
}


static inline void mcu_UsartSetBaudRate( uint32_t u32_UartBaudRate)
{
	unsigned int u32_BaudPreScale = (((F_CPU / (u32_UartBaudRate * 16UL))) - 1);
	REG->UBRR0.USART_BAUD = (uint16_t) u32_BaudPreScale;
}


static void mcu_UsartTransmit(char data)
{
    /* Wait until not busy */
    while ( REG->UCSR0A.UDRE0 == 0 );
	REG->UDR0 = data;
    /* Wait until not busy */
	while ( REG->UCSR0A.UDRE0 == 0 );
}

static void mcu_UsartTransmitString(const char *str)
{
	unsigned char j=0;
	
	while (str[j])		/* Send string till null */
	{
		mcu_UsartTransmit(str[j]);	
		j++;
	}
}


static uint8_t mcu_u8_UsartDataAvailable ( void )
{
    return REG->UCSR0A.RXC0;
}



static uint8_t mcu_u8_UsartReceive(void)
{
    while (!mcu_u8_UsartDataAvailable());	
    return REG->UDR0;				
}


/*
static void mcu_UsartReceiveString(char *str)
{
    unsigned char data;
    do
    {
        data = mcu_u8_UsartReceive();
        *str = data;
        str++;
    } while (data != '\0');
}
*/

static void mcu_Usart0InitBuffer(char *pBuffer)
{
	usart0_buffer = pBuffer;
}

static void mcu_Usart0SendData(const char *pTxBuffer)
{
	mcu_UsartTransmitString( pTxBuffer );
}

static void mcu_Usart0IrqService(void)
{
	*usart0_buffer = mcu_u8_UsartReceive();
	if ( ( *usart0_buffer == '\0' ) || ( *usart0_buffer == '\r' ) || ( *usart0_buffer == '\n' ) )
	{
		usart0_buffer++;
		*usart0_buffer = '\0';
		usart0_buffer -= usart0_buffer_len + 1;
		usart0_buffer = 0;
	}
	else
	{
		usart0_buffer_len++;
		usart0_buffer++;
	}
}