/*---------------------------------------------------------
Includes
---------------------------------------------------------*/
#include "ajm_types.h"

#include "hal_uart.h"
#include "iop_uart.h"

#include "ajm_cstd.h"

/*---------------------------------------------------------
Constants
---------------------------------------------------------*/
#define COMMAND_CT      ( 1 )

/*---------------------------------------------------------
Types
---------------------------------------------------------*/
typedef int (* funcptr)( void );

typedef enum                    /* States for UART Commands         */
{
    CMD_WAIT,                   //Waiting for complete command
    CMD_PROC                    //Command ready for processing
} uart_command_state;

typedef struct                  //Structure for command dictionary
{
    uint8_t    len;               //Length of command
    char    cmd[8];             //Command string
    funcptr proc_fn;            //Callback function for command
} command_dict;

/*---------------------------------------------------------
Local Variables
---------------------------------------------------------*/
char command_buffer[ IOP_UART_BUFFER_SIZE ];
char temp_in_buffer[ HAL_UART_BUFFER_SIZE ];

uint8_t cmd_buf_loc;

static uart_command_state    cur_uart_state;                   /* Current State                   */

/*---------------------------------------------------------
Constant String Library
---------------------------------------------------------*/
static const char       uart_size_error_message[] = "Error! Input Too Long!";
static const char       uart_cmd_error_message[]  = "Error! Invalid Command!";
static const char       hello_world_message[]    = "Hello World!";
static const char       crlf[]                   = "\r\n";

/*---------------------------------------------------------
Command Dictionary

NOTE: All callbacks take no arguments and return nothing
---------------------------------------------------------*/
void tmp_nop( void );
void reset_cmd_buf( void );

static command_dict     cmds[ COMMAND_CT ] = {
    {
        3,
        "nop",
        tmp_nop
    }
};

/*---------------------------------------------------------
Functions
---------------------------------------------------------*/
void reset_cmd_buf( void )
{
    uint8_t i;

    cur_uart_state = CMD_WAIT;
    cmd_buf_loc = 0;

    memset( command_buffer, 0, IOP_UART_BUFFER_SIZE );
}

void tmp_nop( void )
{
    hal_uart_put( "NOP!", 4 );
}

void iop_uart_init( void )
{
    memset( command_buffer, 0, IOP_UART_BUFFER_SIZE );
    memset( temp_in_buffer, 0, HAL_UART_BUFFER_SIZE );

    cmd_buf_loc = 0;

    return;
}

void iop_uart_periodic( void )
{
    uint8_t temp_sz, i;

    temp_sz = hal_uart_buffer_get( temp_in_buffer );

    for( i=0; i<temp_sz; i++ )
    {
        if( cmd_buf_loc < IOP_UART_BUFFER_SIZE )
        {
            //Carriage Return is sign of "execute command"
            if( temp_in_buffer[ i ] == 0xD )
            {
                cur_uart_state = CMD_PROC;
                break;
                //TODO: catch rest of buffer?
            }
            //Otherwise save off the input
            else
            {
                command_buffer[ cmd_buf_loc ] = temp_in_buffer[ i ];
                cmd_buf_loc++;
            }
        }
        else
        {
            hal_uart_put( uart_size_error_message,
                            sizeof( uart_size_error_message ) );
            hal_uart_put( crlf, sizeof( crlf ) );
            reset_cmd_buf();
        }
    }

    //Process Commands if necessary
    if( cur_uart_state == CMD_PROC )
    {
        uint8_t i;
        funcptr command;

        hal_uart_put( crlf, sizeof( crlf ) );

        command = NULL;

        //Iterate through the command dictionary, attempt to find match
        for( i=0; i<COMMAND_CT; i++)
        {
            if( 0 == strncmp( command_buffer, &cmds[i].cmd, cmds[i].len ) )
            {
                command = cmds[i].proc_fn;
                break;
            }
        }

        //Execute command or throw error
        if( command != NULL )
        {
            (void*)( * command )( );
        }
        else
        {
            hal_uart_put( uart_cmd_error_message,
                          sizeof( uart_cmd_error_message ) );
        }

        hal_uart_put( crlf, sizeof( crlf ) );

        //Reset Command Buffer
        reset_cmd_buf();

        hal_uart_put('M', 1);
    }

}