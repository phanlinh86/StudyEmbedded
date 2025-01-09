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
