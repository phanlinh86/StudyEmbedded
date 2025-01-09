#pragma once

static inline void mcu_SetGpioOutput( char* pinString );
static inline void mcu_SetGpioInput( char* pinString );
static inline void mcu_SetGpioHigh( char* pinString );
static inline void mcu_SetGpioLow( char* pinString );
static inline void mcu_ToggleGpio( char* pinString );

/********************************************************************************
*									STM32 GPIOA_BASE							*
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