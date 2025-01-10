#pragma once

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
		case 'C':
			REG->DDRC |= 1 << pin;
		case 'D':
			REG->DDRD |= 1 << pin;
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
		case 'C':
			REG->DDRC &= ~(1 << pin);
		case 'D':
			REG->DDRD &= ~(1 << pin);
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