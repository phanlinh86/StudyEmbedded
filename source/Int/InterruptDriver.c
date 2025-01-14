static uint32_t u32_TimerCounter = 0;
static uint32_t u32_TimerStart = 0;
static void ( *pfServiceIrq ) (void) ;

static void is_SetIrqService(void (*pfServiceFunction)(void))
{
	pfServiceIrq = pfServiceFunction;
}

static void DoIrqService(void)
{
	u32_TimerCounter++;
	
		#if ( ( BOARD_ID ==  BOARD_ID_STM32F411E ) || ( BOARD_ID ==  BOARD_ID_ATMEGA328P ) )
	cmd_DoCommandIsr(); 		// Serve command sent from USART. This need to be done in backgroud
								// in case it will intefere interrupt routines
		#endif // BOARD_ID ==  BOARD_ID_STM32F411E, BOARD_ID_ATMEGA328P
		
	( *pfServiceIrq ) () ;			// This for main project
}

void is_SetTimerStart(uint32_t val)
{
	u32_TimerStart = val;
}


void is_SetTimerCounter(uint32_t val)
{
	u32_TimerCounter = val;
}

static uint32_t is_u32_ReadTimerCounter()
{
	return u32_TimerCounter;
}

// Delay function
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