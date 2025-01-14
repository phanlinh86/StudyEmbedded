#define USART_BAUDRATE_9600 9600
#define USART_BAUDRATE_19200 19200
#define USART_BAUDRATE_38400 38400
#define USART_BAUDRATE_57600 57600
#define USART_BAUDRATE_115200 115200
#define USART_BAUDRATE_230400 230400
#define USART_BAUDRATE_460800 460800
#define USART_BAUDRATE_500000 500000
#define USART_BAUDRATE_921600 921600
#define USART_BAUDRATE_2250000 2000000
#define USART_BAUDRATE_4500000 3000000
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

#define CMD_HEADER 	("/cmd")
#define MAX_CMD_FRAME_LENGTH 		5 		// Each frame contains 32bits data	

#define TX_USART_BUFFER 		( (MAX_CMD_FRAME_LENGTH-1) * 4 + 2 )		// 1 BYTE Status. 4 bytes response. 1 byte termination
#define RX_USART_BUFFER			( 4 + MAX_CMD_FRAME_LENGTH * 4 + 1 ) 	// 4 BYTES COMMAND HEADER. 5 BYTES COMMAND FRAME. 1 BYTE FOR TERMINATION
																		// This need to be adjusted according to MAX_CMD_FRAME_LENGTH in cmd.c

												

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


typedef enum
{
    CMD_IDLE = 0,
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

extern cmd_status eCmdStatus;
static cmd_frame_st cmd_frame;
static resp_frame_st resp_frame;

static cmd_status cmd_eGetCmdStatus(void);
static void cmd_DoCommandIsr(void);
static void cmd_DoCommand(void);
static void cmd_UpdateCommandParam(void);
uint32_t cmd_GetCommandParam( uint8_t paramIndex);
static bool cmd_bCheckCommandInUart(void);
static uint32_t cmd_GetCommand(void);
static void cmd_ResetCommandFrame(void);
static void cmd_ReadRam(void);
uint32_t cmd_GetResp(uint8_t paramIndex);
uint32_t cmd_GetRespStatus(void);
static void cmd_WriteRam(void);
static void cmd_UpdateCommandResponse(void);



