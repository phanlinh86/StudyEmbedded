#define USART_BAUDRATE_9600 9600
#define USART_BAUDRATE_19200 19200
#define USART_BAUDRATE_38400 38400
#define USART_BAUDRATE_57600 57600
#define USART_BAUDRATE_115200 115200
#define USART_BAUDRATE_230400 230400
#define USART_BAUDRATE_460800 460800
#define USART_BAUDRATE_500000 500000
#define USART_BAUDRATE_921600 921600
#define USART_BAUDRATE_1000000 1000000
#define USART_BAUDRATE_2000000 2000000
#define USART_BAUDRATE_4500000 3000000
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1


// I2C application state
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

// I2C CLock Speed
#define I2C_SCL_SPEED_100K 	100000
#define I2C_SCL_SPEED_400K 	400000
#define I2C_SCL_SPEED_200K  200000

// I2C Fast Mode duty cycle
#define I2C_FM_DUTY_2        0
#define I2C_FM_DUTY_16_9     1

#define CMD_HEADER 	("/cmd")
#define MAX_CMD_FRAME_LENGTH 		5 		// Each frame contains 32bits data	

#define TX_USART_BUFFER 		( (MAX_CMD_FRAME_LENGTH-1) * 4 + 2 )		// 1 BYTE Status. 4 bytes response. 1 byte termination
#define RX_USART_BUFFER			( 4 + MAX_CMD_FRAME_LENGTH * 4 + 1 ) 	// 4 BYTES COMMAND HEADER. 5 BYTES COMMAND FRAME. 1 BYTE FOR TERMINATION
																		// This need to be adjusted according to MAX_CMD_FRAME_LENGTH in cmd.c

	#if ( BOARD_ID ==  BOARD_ID_STM32F411E )												
#define MAX_BATCH_DATA 				512 		// 512 * 32 bits = 2K bytes RAM 
	#elif ( BOARD_ID ==  BOARD_ID_STM32L010RB )
#define MAX_BATCH_DATA 				256 		// 256 * 32 bits = 1K bytes RAM	
	#elif ( BOARD_ID ==  BOARD_ID_ATMEGA328P )
#define MAX_BATCH_DATA 				256 		// 256 * 32 bits = 1K bytes RAM
	#endif // BOARD_ID == ?

typedef enum
{
	USART_NO_STOP_BITS_0p5 	= 0,
	USART_NO_STOP_BITS_1p0	= 1,
	USART_NO_STOP_BITS_1p5 	= 2,
	USART_NO_STOP_BITS_2p0 	= 3,
} usart_nostopbit;

typedef enum
{
	USART_MODE_ONLY_TX 		= 0,
	USART_MODE_ONLY_RX 		= 1,
	USART_MODE_TXRX  		= 2,
} usart_mode;

typedef enum
{
	USART_PARITY_DISABLE 	= 0,
	USART_PARITY_EN_ODD 	= 1,
	USART_PARITY_EN_EVEN 	= 2,
} usart_parity_mode;

typedef struct 
{
	usart_mode eMode;
	usart_nostopbit eNoOfStopBits;
	usart_parity_mode eParityControl;
	uint32_t u32_BaudRate;
	uint8_t u8_WordLength;
} usart_config;

extern uint8_t uart_rx_buffer[RX_USART_BUFFER];
extern uint8_t uart_tx_buffer[TX_USART_BUFFER];
extern bool bUartRxComplete;
extern uint32_t batch_data[MAX_BATCH_DATA];             // 1K for large data processing.
extern uint16_t u16_batch_idx;                          // Index for batch data processing

typedef struct
{
	uint32_t u32_ClockSpeed;
	uint8_t  u8_Address;
	uint8_t  u8_AckControl;
	uint8_t  u8_DutyCycle;

}i2c_config;

typedef enum
{
    CMD_IDLE = 0,
    CMD_PROCESSING = 2,
    CMD_COMPLETE = 3,
} cmd_status;


//  COMMAND TYPE
// // 0x00xx - Basic functions
#define GET_INFO		0x0000
#define WRITE_RAM		0x0001
#define	READ_RAM		0x0002

// 0x01xx - Write/Read Hardware
#define WRITE_GPIO		0x0101
#define READ_GPIO		0x0102
#define WRITE_I2C		0x0103
#define READ_I2C		0x0104

// 0x02xx - Send/Capture data
// These group of command requires more than 1 ISR to complete execution
#define CAPTURE_DATA8	0x0201
#define CAPTURE_DATA16	0x0202
#define CAPTURE_DATA32	0x0204

#define SEND_DATA		0x0210

// 0xFxxx - System command
#define SOFT_RESET 		0xFEEF
	

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

extern cmd_status eCmdStatus;
extern cmd_frame_st cmd_frame;
extern resp_frame_st resp_frame;

static cmd_status cmd_eGetCmdStatus(void);
static void cmd_DoCommandIsr(void);
static void cmd_DoCommand(void);
static void cmd_UpdateCommandParam(void);
uint32_t cmd_GetCommandParam( uint8_t paramIndex);
static bool cmd_bCheckCommandInUart(void);
static uint32_t cmd_GetCommand(void);
static void cmd_ResetCommandFrame(void);

uint32_t cmd_GetResp(uint8_t paramIndex);
uint32_t cmd_GetRespStatus(void);
static void cmd_UpdateCommandResponse(void);

static void cmd_WriteRam(void);
static void cmd_ReadRam(void);
static void cmd_GetInfo(void);
static void cmd_WriteGpio(void);
static void cmd_ReadGpio(void);
static void cmd_ReadI2c(void);
static void cmd_WriteI2c(void);

static void cmd_SoftReset(void);

static void cmd_CaptureData(uint8_t u8_ByteSize);
static void cmd_SendData(void);
static uint16_t cmd_GetBatchIndex(void);
static void cmd_ResetBatchIndex(void);

