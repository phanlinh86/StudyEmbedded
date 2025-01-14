static void ( *pfServiceIrq ) (void) ;			// General IRQ Service from a dedicated Timer
static void is_SetIrqService(void (*pfServiceFunction)(void));
static void DoIrqService(void);
void is_SetTimerCounter(uint32_t val);
static uint32_t is_u32_ReadTimerCounter();
void is_SetTimerStart(uint32_t val);
void DelayUs( uint32_t u32_TimeInUs );
void DelayMs( uint32_t u32_TimeInMs );
void Delay( uint32_t u32_DelayLoop );
