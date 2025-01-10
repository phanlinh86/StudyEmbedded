#include <avr/interrupt.h>

static uint32_t u32_Timer1Counter = 0;
static uint32_t u32_Timer1Start = 0;


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

static uint32_t is_u32_ReadTimer1Counter()
{
	return u32_Timer1Counter;
}

static void is_SetTimer1Counter( uint32_t val)
{
	u32_Timer1Counter = val;
}

static void is_InitTimer1(void)
{
    cli();                                 		// Disable interrupts
    mcu_ResetTimer1();							// Reset timer1
    u32_Timer1Start = TIMER1_RESOLUTION - mcu_SetTimer1Clk(TIMER1_PERIOD_IN_US);   	// Set timer1 to 1ms
    mcu_EnableTimer1Interrupt();
    sei();                      				// Enable interrupts
}