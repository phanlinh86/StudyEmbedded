/* This is for command packge which communicate with the shell
 * and execute the command 
 
1. CMD_IDLE. Keep reading the FIFO (Ring buffer). If FIFO is not empty, and in the format of 
	a. 33 byte package
	b. First byte contains one of command header
	then proceed to the next stage
2. CMD_WAITING. 
	Getting information from the command 
	Byte		Description
	 0 				CMD 	
	 1				PARAM0
	 2				PARAM1
	 3				PARAM2
	 4				PARAM3
	 5				PARAM4
	 6				PARAM5
	 7				PARAM6
	 8				PARAM7
	 ...
	 31				PARAM31

 */
 
uint8_t uart_rx_buffer[RX_USART_BUFFER];
uint8_t uart_tx_buffer[TX_USART_BUFFER];
cmd_frame_st cmd_frame;
resp_frame_st resp_frame;

#define reg8( r ) unsigned char r

static cmd_status cmd_eGetCmdStatus(void)
{
    return eCmdStatus;
}

static void cmd_SetCmdStatus(cmd_status eStatus)
{
    eCmdStatus = eStatus;
}

static void cmd_DoCommandIsr(void)
{
	switch (cmd_eGetCmdStatus())
	{
		case CMD_IDLE:
			// Check first 2 bytes of UART buffer whether matches with required command header to move to next stage
			if ( cmd_bCheckCommandInUart() == TRUE )
				cmd_SetCmdStatus(CMD_PROCESSING);
			break;
		case CMD_PROCESSING:
			cmd_UpdateCommandParam();			// Get command parameter from the buffer
			ut_ResetRxBuffer();					// Reset receive buffer after proccessing Rx Buffer
			cmd_DoCommand();					// Serve the command
			cmd_UpdateCommandResponse();		// Update the tx buffer using command response
			cmd_ResetCommandFrame();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;			
		case CMD_COMPLETE:
			ut_SendUart(uart_tx_buffer);		// Send response through USART
			ut_ResetTxBuffer();					// Reset tx buffer after sent	
			cmd_SetCmdStatus(CMD_IDLE);			
			break;			
	}
}

static bool cmd_bCheckCommandInUart(void)
{
	// Check first 2 bytes of UART buffer whether matches with required command header to move to next stage
	return  ( uart_rx_buffer[0] == CMD_HEADER[0] ) && 
			( uart_rx_buffer[1] == CMD_HEADER[1] ) &&
			( uart_rx_buffer[2] == CMD_HEADER[2] ) &&
			( uart_rx_buffer[3] == CMD_HEADER[3] ) &&
			(bUartRxComplete == TRUE);
}

static void cmd_UpdateCommandParam(void)
{
	cmd_frame.command = ((uint32_t) uart_rx_buffer[4] << 24) + ((uint32_t) uart_rx_buffer[5] << 16) + ((uint32_t) uart_rx_buffer[6] << 8) + uart_rx_buffer[7];
	cmd_frame.param0 =  ((uint32_t) uart_rx_buffer[8] << 24) + ((uint32_t) uart_rx_buffer[9] << 16) + ((uint32_t) uart_rx_buffer[10] << 8) + uart_rx_buffer[11]; 
	cmd_frame.param1 = ((uint32_t) uart_rx_buffer[12] << 24) + ((uint32_t) uart_rx_buffer[13] << 16) + ((uint32_t) uart_rx_buffer[14] << 8) + uart_rx_buffer[15]; 
	cmd_frame.param2 = ((uint32_t) uart_rx_buffer[16] << 24) + ((uint32_t) uart_rx_buffer[17] << 16) + ((uint32_t) uart_rx_buffer[18] << 8) + uart_rx_buffer[19]; 
	cmd_frame.param3 = ((uint32_t) uart_rx_buffer[20] << 24) + ((uint32_t) uart_rx_buffer[21] << 16) + ((uint32_t) uart_rx_buffer[22] << 8) + uart_rx_buffer[23]; 
}

static void cmd_UpdateCommandResponse(void)
{
	uart_tx_buffer[0] = (uint8_t) resp_frame.status;
	// Resp0
	uart_tx_buffer[1] = (uint8_t) ( resp_frame.resp0 >> 24 );
	uart_tx_buffer[2] = (uint8_t) ( ( resp_frame.resp0 >> 16 ) & 0xFF );
	uart_tx_buffer[3] = (uint8_t) ( ( resp_frame.resp0 >> 8 ) & 0xFF );
	uart_tx_buffer[4] = (uint8_t) ( resp_frame.resp0 & 0xFF );
	// Resp1
	uart_tx_buffer[5] = (uint8_t) ( resp_frame.resp1 >> 24 );
	uart_tx_buffer[6] = (uint8_t) ( ( resp_frame.resp1 >> 16 ) & 0xFF );
	uart_tx_buffer[7] = (uint8_t) ( ( resp_frame.resp1 >> 8 ) & 0xFF );
	uart_tx_buffer[8] = (uint8_t) ( resp_frame.resp1 & 0xFF );	
	// Resp2
	uart_tx_buffer[9] = (uint8_t) ( resp_frame.resp2 >> 24 );
	uart_tx_buffer[10] = (uint8_t) ( ( resp_frame.resp2 >> 16 ) & 0xFF );
	uart_tx_buffer[11] = (uint8_t) ( ( resp_frame.resp2 >> 8 ) & 0xFF );
	uart_tx_buffer[12] = (uint8_t) ( resp_frame.resp2 & 0xFF );	
	// Resp3
	uart_tx_buffer[13] = (uint8_t) ( resp_frame.resp3 >> 24 );
	uart_tx_buffer[14] = (uint8_t) ( ( resp_frame.resp3 >> 16 ) & 0xFF );
	uart_tx_buffer[15] = (uint8_t) ( ( resp_frame.resp3 >> 8 ) & 0xFF );
	uart_tx_buffer[16] = (uint8_t) ( resp_frame.resp3 & 0xFF );
	uart_tx_buffer[17] = '\n';
}

uint32_t cmd_GetCommandParam( uint8_t paramIndex)
{
	uint32_t *pBuffer = (uint32_t*) &cmd_frame + 1; 
	return *(pBuffer+paramIndex);
}

static uint32_t cmd_GetCommand(void)
{
	return cmd_frame.command;
}

uint32_t cmd_GetRespStatus(void)
{
	return resp_frame.status;
}

uint32_t cmd_GetResp(uint8_t paramIndex)
{
	uint32_t *pBuffer = (uint32_t*) &resp_frame + 1; 
	return *(pBuffer+paramIndex);	
}

static void cmd_ResetCommandFrame(void)
{
	uint32_t *pBuffer = (uint32_t*) &cmd_frame; 
	for (int i=0; i < MAX_CMD_FRAME_LENGTH; i++ )
		*(pBuffer+i) = 0;
}

static void cmd_DoCommand(void)
{
	switch ( cmd_GetCommand() )
	{		
		// Basic functions : 0x00xx
		case GET_INFO:				// Get board information 	0x0000
			cmd_GetInfo();
			break;		
		case WRITE_RAM:				// Write ram				0x0001
			cmd_WriteRam();
			break;
		case READ_RAM:				// Read ram					0x0002
			cmd_ReadRam();		
			break;
			
		//  Hardware
		case WRITE_GPIO:			// Write GPIO				0x0101
			cmd_WriteGpio();
			break;
		case READ_GPIO:				// Read GPIO				0x0102
			cmd_ReadGpio();
			break;

		// Reset
		case SOFT_RESET:
			cmd_SoftReset();
			break;
	}
}


// Command package

// 0x00xx 	- 	BASIC
static void cmd_GetInfo(void)
{
	resp_frame.status = 1;
	switch ( cmd_frame.param0 ) 
	{
		case 0x0000:			// Board information
			resp_frame.resp0 	= 	BOARD_ID; 
			break;
		case 0x0001:			// Project information
			resp_frame.resp0 	= 	PROJECT;
			break;
		case 0x0002:			// CPU Clock
			resp_frame.resp0 	=	FPU;
			break;
		case 0x0003: 			// Firmware Revision
			resp_frame.resp0 	=	FIRMWARE_REVISION;
			break;
			
		case 0x0100:			// USART
			if ( cmd_frame.param1 == 0 )
			{
				resp_frame.resp0 	=	(uint32_t) ut_u16_GetUsedUsart();
			}
			else
			{
				usart_config UsartConfig = ut_GetUsartConfig();
				resp_frame.resp0	=   (uint32_t) UsartConfig.u32_BaudRate; 
				resp_frame.resp1	= 	(uint32_t) UsartConfig.u8_WordLength; 
				resp_frame.resp2	= 	(uint32_t) UsartConfig.eNoOfStopBits; 
				resp_frame.resp3 	=	(uint32_t) UsartConfig.eParityControl; 
			}
			break;
			
	}
}

static void cmd_ReadRam(void)
{	
	uint32_t *pTemp;
	pTemp = (uint32_t*) (uintptr_t)cmd_frame.param0;
	resp_frame.status 	= 1;
	switch ( cmd_frame.param1 ) 
	{
		case 4: 	// 4 bytes - 32 bits
			resp_frame.resp0 	= *( pTemp );
			break;
		case 2:		// 2 byte - 16 bits
			resp_frame.resp0 	= (uint16_t) *( pTemp ) & 0xFFFF;
			break;
		case 1:		// 1 byte - 8 bits
			resp_frame.resp0 	= (uint8_t) *( pTemp ) & 0xFF;
			break;
		default:	// Other option - 32 bits by default
			resp_frame.resp0 	= *( pTemp );
			break;		
	}
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}

static void cmd_WriteRam(void)
{	
	uint32_t *pTemp;
	pTemp = (uint32_t*)(uintptr_t) cmd_frame.param0;
	resp_frame.status 	= 1;

	switch ( cmd_frame.param2 ) 
	{
		case 4: 	// 4 bytes - 32 bits
			resp_frame.resp0 	= *( pTemp );
			*( pTemp ) = cmd_frame.param1;
			break;
		case 2:		// 2 byte - 16 bits
			resp_frame.resp0 	= (uint16_t) *( pTemp ) & 0xFFFF;
			*( pTemp ) = ( *( pTemp ) & 0xFFFF0000 ) |  cmd_frame.param1;
			break;
		case 1:		// 1 byte - 8 bits
			resp_frame.resp0 	= (uint8_t) *( pTemp ) & 0xFF;
			*( pTemp ) = ( *( pTemp ) & 0xFFFFFF00 ) |  cmd_frame.param1;
			break;
		default:	// Other option - 32 bits by default
			*( pTemp ) 			= cmd_frame.param1;
			resp_frame.resp0 	= *( pTemp );
			break;		
	}
	resp_frame.resp1    = cmd_frame.param1;
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}


// 0x00xx 	- 	HARDWARE
static void cmd_WriteGpio(void)
{
	char pTemp[4];
	pTemp[0] = cmd_frame.param0 >> 24;
	pTemp[1] = ( cmd_frame.param0 >> 16 ) & (0xFF);
	pTemp[2] = ( cmd_frame.param0 >> 8 ) & (0xFF);
	pTemp[3] = cmd_frame.param0 & 0xFF;

	switch ( cmd_frame.param1 )
	{
		case 0:
			mcu_SetGpioLow(pTemp);			// Set Low
			break;
		case 1:
			mcu_SetGpioHigh(pTemp);			// Set High
			break;
		case 2:
			mcu_ToggleGpio(pTemp);			// Toggle
			break;
		case 3:
			mcu_SetGpioOutput(pTemp);		// Set output
			break;
		case 4:
			mcu_SetGpioInput(pTemp);		// Set input
			break;
				#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
		case 5:
			mcu_SetGpioAlternate(pTemp, cmd_frame.param2);			// Set Alternate pin which read from param2
			break;
				#endif // BOARD_ID == BOARD_ID_STM32F411E
	}
	
	resp_frame.status 	= 1;
	resp_frame.resp0 	= cmd_frame.param0;
	resp_frame.resp1 	= cmd_frame.param1;
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}

static void cmd_ReadGpio(void)
{
	char pTemp[4];
	uint8_t u8_Value;
	pTemp[0] = cmd_frame.param0 >> 24;
	pTemp[1] = ( cmd_frame.param0 >> 16 ) & (0xFF);
	pTemp[2] = ( cmd_frame.param0 >> 8 ) & (0xFF);
	pTemp[3] = cmd_frame.param0 & 0xFF;

	u8_Value = mcu_u8_ReadGpio(pTemp);
	
	resp_frame.status 	= 1;
	resp_frame.resp0 	= u8_Value;
	resp_frame.resp1 	= 0x00;
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}

static void cmd_SoftReset(void)
{
	mcu_SoftReset();
	
	resp_frame.status 	= 1;
	resp_frame.resp0 	= 0x00;
	resp_frame.resp1 	= 0x00;
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}