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
uint32_t batch_data[MAX_BATCH_DATA];                // 1K for large data processing.
uint16_t u16_batch_idx = 0;                         // Index for batch data processing
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
			{
				cmd_SetCmdStatus(CMD_PROCESSING);
			    cmd_UpdateCommandParam();			// Get command parameter from the buffer
			    ut_ResetRxBuffer();					// Reset receive buffer after proccessing Rx Buffer
			    cmd_ResetBatchIndex();              // Reset batch index
            }
			break;
		case CMD_PROCESSING:
			cmd_DoCommand();					// Serve the command
			break;			
		case CMD_COMPLETE:
		    cmd_ResetCommandFrame();            // Reset command frame after complete
			cmd_UpdateCommandResponse();		// Update the tx buffer using command response		
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
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
		case WRITE_RAM:				// Write ram				0x0001
			cmd_WriteRam();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
		case READ_RAM:				// Read ram					0x0002
			cmd_ReadRam();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
			
		//  Hardware
		case WRITE_GPIO:			// Write GPIO				0x0101
			cmd_WriteGpio();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
		case READ_GPIO:				// Read GPIO				0x0102
			cmd_ReadGpio();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
				#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
		case WRITE_I2C:				// Write I2C				0x0103
			cmd_WriteI2c();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
		case READ_I2C:				// Read I2C					0x0104
			cmd_ReadI2c();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
				
		case READ_TEMP:				// Read Temperature
			cmd_ReadTemp();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;		
		case READ_ACCEL:				// Read Accelerometer
			cmd_ReadAccel();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;
		case READ_MAGNET:				// Read Magnetometer
			cmd_ReadMagnet();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;				
				#endif // BOARD_ID ==  BOARD_ID_STM32F411E


		// Reset
		case SOFT_RESET:
			cmd_SoftReset();
			cmd_SetCmdStatus(CMD_COMPLETE);
			break;

		// Send/Capture data
		case CAPTURE_DATA8:				// Capture data				0x0201
            cmd_CaptureData(1);
            break;
		case CAPTURE_DATA16:			// Capture data				0x0202
            cmd_CaptureData(2);
            break;
		case CAPTURE_DATA32:			// Capture data				0x0204
            cmd_CaptureData(4);
            break;
			
        case SEND_DATA:				// Send data				0x0210
            cmd_SendData();
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
		#if ( BOARD_ID ==  BOARD_ID_STM32F411E )
static void cmd_WriteI2c(void)
{
	uint8_t u8_DeviceAddress;
	uint8_t u8_SlaveAddress;
	uint8_t u8_SlaveValue;
	
	u8_DeviceAddress = cmd_frame.param0 & 0xFF;
	u8_SlaveAddress = cmd_frame.param1 & 0xFF;
	u8_SlaveValue = cmd_frame.param2 & 0xFF;


	ut_WriteI2c(u8_DeviceAddress, u8_SlaveAddress, u8_SlaveValue);

	resp_frame.status 	= 1;
	resp_frame.resp0 	= u8_SlaveValue;
	resp_frame.resp1 	= u8_DeviceAddress;
	resp_frame.resp2 	= u8_SlaveAddress;
	resp_frame.resp3 	= 0x00;		
}

static void cmd_ReadI2c(void)
{
	uint8_t u8_DeviceAddress;
	uint8_t u8_SlaveAddress;
	uint8_t u8_SlaveLen;
	uint8_t u8_SlaveValue;	
	
	u8_DeviceAddress 	= cmd_frame.param0 & 0xFF;
	u8_SlaveAddress 	= cmd_frame.param1 & 0xFF;
	u8_SlaveLen 		= cmd_frame.param2 & 0xFF;

	u8_SlaveValue = ut_ReadI2c(u8_DeviceAddress, u8_SlaveAddress, u8_SlaveLen);

	resp_frame.status 	= 1;
	resp_frame.resp0 	= u8_SlaveValue;
	resp_frame.resp1 	= u8_DeviceAddress;
	resp_frame.resp2 	= u8_SlaveAddress;
	resp_frame.resp3 	= 0x00;	
}

static void cmd_ReadTemp(void)
{
	ut_ReadI2c(ACCEL_SENSOR_SLAVE_ADDR, OUT_TEMP_A_REG, 2);

	resp_frame.status 	= 1;
	resp_frame.resp0 	= batch_data[0] & 0xFFFF;
	resp_frame.resp1 	= ACCEL_SENSOR_SLAVE_ADDR;
	resp_frame.resp2 	= OUT_TEMP_A_REG;
	resp_frame.resp3 	= 0x00;		
}

static void cmd_ReadAccel(void)
{
	ut_ReadI2c(ACCEL_SENSOR_SLAVE_ADDR, OUT_X_A_REG, 6);

	resp_frame.status 	= 1;
	resp_frame.resp0 	= batch_data[0] & 0xFFFF;				// accel_x
	resp_frame.resp1 	= ( batch_data[0] & 0xFFFF0000 ) >> 16; // accel_y
	resp_frame.resp2 	= batch_data[1] & 0xFFFF;				// accel_z
	resp_frame.resp3 	= 0x00;			
}

static void cmd_ReadMagnet(void)
{
	ut_ReadI2c(MAGNET_SENSOR_SLAVE_ADDR, OUTX_REG_M_REG, 6);

	resp_frame.status 	= 1;
	resp_frame.resp0 	= batch_data[0] & 0xFFFF;				// magnet_x
	resp_frame.resp1 	= ( batch_data[0] & 0xFFFF0000 ) >> 16; // magnet_y
	resp_frame.resp2 	= batch_data[1] & 0xFFFF;				// magnet_z
	resp_frame.resp3 	= 0x00;			
}
		#endif // ( BOARD_ID ==  BOARD_ID_STM32F411E )

// 
static void cmd_SoftReset(void)
{
	mcu_SoftReset();
	
	resp_frame.status 	= 1;
	resp_frame.resp0 	= 0x00;
	resp_frame.resp1 	= 0x00;
	resp_frame.resp2 	= 0x00;
	resp_frame.resp3 	= 0x00;
}

// Capture / Read batch data
static void cmd_CaptureData(uint8_t u8_ByteSize)
{
	static volatile uint32_t *pBuffer;
	static volatile uint8_t *pBatch8;		// 8-bits Pointer to program Batch Data
	uint32_t u32_VarVal;					// Variable value
	if ( cmd_GetBatchIndex() == 0 )
	{		
		pBuffer = (uint32_t*) &cmd_frame + 1;
		pBatch8 = (uint8_t*)  &batch_data;	
	}
	
    // Capture data from ISR and store in the batch data
	for ( int i = 0; i < 4; i++ )
	{
		if (*( pBuffer + i ) != 0 )
		{
			u32_VarVal = *((uint32_t*)(uintptr_t) *( pBuffer + i ));	// Value of variable i
			switch ( u8_ByteSize )
			{
				case 1:
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF );
					break;
				case 2:
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF );
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF00 ) >> 8;
					break;
				case 4:
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF );
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF00 ) >> 8;
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF0000 ) >> 12;
					*( pBatch8 + u16_batch_idx++) = (u32_VarVal & 0xFF000000 ) >> 24;
					break;
			}
		}
	}
	 	
    if ( u16_batch_idx >= MAX_BATCH_DATA * 4 )
    {
        // Reset the index
        cmd_ResetBatchIndex();
        cmd_SetCmdStatus(CMD_COMPLETE);
	    resp_frame.status 	= 1;
	    resp_frame.resp0 	= 0x00;
	    resp_frame.resp1 	= 0x00;
	    resp_frame.resp2 	= 0x00;
	    resp_frame.resp3 	= 0x00;
    }
}

static void cmd_SendData(void)
{
    // Send data to the sensor
    u16_batch_idx ++;
}


static uint16_t cmd_GetBatchIndex(void)
{
    return u16_batch_idx;
}

static void cmd_ResetBatchIndex(void)
{
    u16_batch_idx = 0;
}