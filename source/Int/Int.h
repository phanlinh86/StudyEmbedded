extern static void ( *pfServiceIrq ) (void) ;			// General IRQ Service from a dedicated Timer
static void is_SetIrqService(void (*pfServiceFunction)(void));
static void DoIrqService(void);
static void is_SetTimer1Counter(uint32_t val);
static uint32_t is_u32_ReadTimer1Counter();
static void is_SetTimerStart(uint32_t val);




