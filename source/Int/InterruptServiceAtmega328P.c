#include <avr/interrupt.h>

static uint32_t u32_Timer1Counter = 0;
static uint32_t u32_Timer1Start = 0;
static void ( *pfServiceUsart0Irq ) (void) ; 		// Pointer to handle interrupt USART6 

#define TIMER1_PERIOD_IN_US 		1000

// #Delay
void DelayUs( uint32_t u32_TimeInUs )
{
	for (uint32_t volatile i = 0; i < ( u32_TimeInUs * FPU / 7 ); i++);
}

void DelayMs( uint32_t u32_TimeInMs )
{
	for (uint32_t volatile i = 0; i < ( u32_TimeInMs * 1000 * FPU / 7  ); i++);
}

void Delay( uint32_t u32_DelayLoop )
{
	for (uint32_t volatile i = 0; i < u32_DelayLoop; i++);
}

/********************************************************************************
 * 									ARM SYS TICK								*
 * 								System Tick Timer								*
 * ******************************************************************************/
ISR (TIMER1_OVF_vect)
{
    u32_Timer1Counter++;
	TCNT1 = u32_Timer1Start; 		// Timer start from this value to max defined in TIMER1_RESOLUTION
}

ISR (USART_RX_vect)
{
	(*pfServiceUsart0Irq)();
}

static uint32_t is_u32_ReadTimer1Counter()
{
	return u32_Timer1Counter;
}

static void is_SetTimer1Counter( uint32_t val)
{
	u32_Timer1Counter = val;
}

static void is_InitTimer1(uint32_t u32_Timer1PeriodInMs)
{
    cli();                                 		// Disable interrupts
    mcu_ResetTimer1();							// Reset timer1
    u32_Timer1Start = TIMER1_RESOLUTION - mcu_SetTimer1Clk(u32_Timer1PeriodInMs*1000);   	// Set timer1 to 1ms
    mcu_EnableTimer1Interrupt();
    sei();                      				// Enable interrupts
}

static void is_SetUsart0Interrupt(void)
{
	// Do nothing. Arduino doesn't need to set NVIC bits
}

static void is_InitUsart0Isr(void (*pfServiceFunction)(void))
{
	pfServiceUsart0Irq = pfServiceFunction;
}