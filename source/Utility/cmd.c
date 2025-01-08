/* This is for command packge which communicate with the shell
 * and execute the command */

typedef enum
{
    CMD_IDLE = 0,
    CMD_WAITING = 1,
    CMD_PROCESSING = 2,
    CMD_COMPLETE = 3,
} cmd_status;

cmd_status eCmdStatus = CMD_IDLE;

static bool cmd_IsCmdComplete(void)
{
    return (eCmdStatus == CMD_COMPLETE);
}
