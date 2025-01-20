#include 	<avr/interrupt.h>
#include 	<avr/wdt.h>

static uint32_t u32_Timer1Counter = 0;
static uint32_t u32_Timer1Start = 0;
static void ( *pfServiceUsart0Irq ) (void) ; 		// Pointer to handle interrupt USART6 

#define TIMER1_PERIOD_IN_US 		1000

/********************************************************************************
 * 									ARM SYS TICK								*
 * 								System Tick Timer								*
 * ******************************************************************************/
ISR (TIMER1_OVF_vect)
{
	TCNT1 = u32_Timer1Start; 		// Timer start from this value to max defined in TIMER1_RESOLUTION
    u32_Timer1Counter++;
	DoIrqService();					// The IRQ do main loop
}

ISR (USART_RX_vect)
{
	(*pfServiceUsart0Irq)();
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


// Do soft reset using Watchdog Timer
static void is_SoftResetUseWdt(void)
{
	cli();
	wdt_reset();
	MCUSR = 0;                           // allow changes, disable reset
	WDTCSR = (1 << WDCE) | (1 << WDE);    //set up WDT interrupt
	WDTCSR = (1 << WDE) | (1 << WDP2);   //Start watchdog timer with 0.25s prescaler
	sei();
	Delay( 2000 ) ;  // force WDT to reset system
}