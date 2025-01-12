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
 
char uart_rx_buffer[MAX_USART_BUFFER];
char uart_tx_buffer[MAX_USART_BUFFER];

#define CMD_HEADER 	("/cmd")
#define MAX_CMD_FRAME_LENGTH 		5 		// Each frame contains 32bits data

#define reg8( r ) unsigned char r

typedef enum
{
    CMD_IDLE = 0,
    CMD_WAITING = 1,
    CMD_PROCESSING = 2,
    CMD_COMPLETE = 3,
} cmd_status;

typedef volatile struct
{
	uint32_t	command;
	uint32_t 	param0;
	uint32_t	param1;
	uint32_t 	param2;
	uint32_t 	param3;
} cmd_frame_st;

typedef volatile struct
{
	uint32_t 	status;
	uint32_t 	resp0;
	uint32_t	resp1;
	uint32_t 	resp2;
	uint32_t 	resp3;
} resp_frame_st;

static cmd_status eCmdStatus = CMD_IDLE;
static cmd_frame_st cmd_frame;
static resp_frame_st resp_frame;
static uint32_t command;

static cmd_status cmd_eGetCmdStatus(void);
static void cmd_DoCommandIsr(void);
static void cmd_DoCommand(void);
static void cmd_UpdateCommandParam(void);
static uint32_t cmd_GetCommandParam( uint8_t paramIndex);
static bool cmd_bCheckCommandInUart(void);
static uint32_t cmd_GetCommand(void);


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
			cmd_UpdateCommandParam();
			// cmd_SetCmdStatus(CMD_PROCESSING);
			// Get command information to process in the next stage
			break;
		case CMD_PROCESSING:
			cmd_DoCommand();
			break;			
		case CMD_COMPLETE:			
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
			( uart_rx_buffer[3] == CMD_HEADER[3] );
			// ( ut_GetRxBufferLen() >= (MAX_CMD_FRAME_LENGTH + 1) * 4 );
}

static void cmd_UpdateCommandParam(void)
{
	cmd_frame.command = (uart_rx_buffer[4] << 12) + (uart_rx_buffer[5] << 8) + (uart_rx_buffer[6] << 4) + uart_rx_buffer[7]; 
	command = (uart_rx_buffer[4] << 12) + (uart_rx_buffer[5] << 8) + (uart_rx_buffer[6] << 4) + uart_rx_buffer[7]; 
	cmd_frame.param0 =  (uart_rx_buffer[8] << 12) + (uart_rx_buffer[9] << 8) + (uart_rx_buffer[10] << 4) + uart_rx_buffer[11]; 
	cmd_frame.param1 = (uart_rx_buffer[12] << 12) + (uart_rx_buffer[13] << 8) + (uart_rx_buffer[14] << 4) + uart_rx_buffer[15]; 
	cmd_frame.param2 = (uart_rx_buffer[16] << 12) + (uart_rx_buffer[17] << 8) + (uart_rx_buffer[18] << 4) + uart_rx_buffer[19]; 
	cmd_frame.param3 = (uart_rx_buffer[20] << 12) + (uart_rx_buffer[21] << 8) + (uart_rx_buffer[22] << 4) + uart_rx_buffer[23]; 
}

static uint32_t cmd_GetCommandParam( uint8_t paramIndex)
{
	uint32_t *pBuffer = (uint32_t*) &cmd_frame + 1; 
	return *(pBuffer+paramIndex);
}

static uint32_t cmd_GetCommand(void)
{
	return cmd_frame.command;
}

static void cmd_DoCommand(void)
{
	switch ( cmd_GetCommand() )
	{
		case 0x0000:				// Read ram
			cmd_SetCmdStatus(CMD_COMPLETE);		
			// *( (uint32_t*) 0x00800100) = u32_LedPeriodInMs / 2;
			ut_SendUart("Here\n");
			break;
			
		case 0x00001:				// Write ram
			break;
	}
	
}