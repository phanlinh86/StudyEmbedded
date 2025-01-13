static uint32_t u32_TimerCounter = 0;
static uint32_t u32_TimerStart = 0;
static void ( *pfServiceIrq ) (void) ;

static void is_SetIrqService(void (*pfServiceFunction)(void))
{
	pfServiceIrq = pfServiceFunction;
}


static void DoIrqService(void)
{
	u32_Timer1Counter++;
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
	cmd_DoCommandIsr(); 		// Serve command sent from USART
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E, BOARD_ID_ATMEGA328P
	Loop();						// Main loop running project
}

static void is_SetTimerStart(uint32_t val)
{
	u32_TimerStart = val;
}


static void is_SetTimer1Counter(uint32_t val)
{
	u32_TimerCounter = val;
}

static uint32_t is_u32_ReadTimer1Counter()
{
	return u32_Timer1Counter;
}

