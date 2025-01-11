// Timer related function
void DelayUs( uint32_t u32_TimeInUs );
void DelayMs( uint32_t u32_TimeInMs );
void Delay( uint32_t u32_DelayLoop );

static uint32_t is_u32_ReadTimer1Counter();
static void is_SetTimer1Counter( uint32_t val);
static void is_InitTimer1(uint32_t u32_Timer1PeriodInMs);
static void is_SetUsart0Interrupt(void);
static void is_InitUsart0Isr(void (*pfServiceFunction)(void));
