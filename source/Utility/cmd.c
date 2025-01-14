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
				cmd_SetCmdStatus(CMD_WAITING);
			break;
		case CMD_WAITING:
			cmd_UpdateCommandParam();			// Get command parameter from the buffer
			ut_ResetRxBuffer();					// Reset receive buffer after proccessing Rx Buffer
			cmd_SetCmdStatus(CMD_PROCESSING);
			// Get command information to process in the next stage
			break;
		case CMD_PROCESSING:
			cmd_DoCommand();					// Serve the command
			cmd_UpdateCommandResponse();		// Update the tx buffer using command response
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;			
		case CMD_COMPLETE:		
			cmd_ResetCommandFrame();
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
		case 0x00001:				// Write ram
			cmd_WriteRam();
			break;
		case 0x0002:				// Read ram			
			cmd_ReadRam();		
			break;
	}
}


static void cmd_ReadRam(void)
{	
	uint32_t *pTemp;
	pTemp = (uint32_t*) (uintptr_t)cmd_frame.param0;
	resp_frame.status 	= 1;
	
	resp_frame.resp0 	= *( pTemp );
}

static void cmd_WriteRam(void)
{	
	uint32_t *pTemp;
	pTemp = (uint32_t*)(uintptr_t) cmd_frame.param0;
	resp_frame.status 	= 1;
	resp_frame.resp0 	= *( pTemp );
	resp_frame.resp1    = cmd_frame.param1;
	*( pTemp ) = cmd_frame.param1;
}