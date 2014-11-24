/*
Hardware Abstraction Layer for UART0 - The UART connected to the ICDI Line
*/

//Includes
#include "ajm_types.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"

#include "ajm_cstd.h"

#include "hal_uart.h"

//Local variables
char uart_buffer[ HAL_UART_BUFFER_SIZE ]; //We shouldnt expect more than 15 bytes/ms max
uint8_t uart_loc;


//Functions
void hal_uart_init( void )
{
    uint8_t i;
    // The following lines borrowed from TI Example 'uart_polled.c'

    // This assumes the sysctl clocks have been set

    // Enable the peripherals used by this example.
    // The UART itself needs to be enabled, as well as the GPIO port
    // containing the pins that will be used.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    //
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);

    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    // TODO: change this to match the port/pin you are using
    //
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

    // Initialize UART Buffer
    memset( uart_buffer, 0, HAL_UART_BUFFER_SIZE );
    uart_loc = 0;


    return;
}

void hal_uart_periodic( void )
{
    char cThisChar;

    //AJM TEMP - Loopback behavior
    //
    // Enter a loop to read characters from the UART, and write them back
    // (echo).  When a line end is received, the loop terminates.
    //
    do
    {
        //
        // Read a character using the blocking read function.  This function
        // will not return until a character is available.
        //
        cThisChar = ROM_UARTCharGet(UART0_BASE);

        if( uart_loc < HAL_UART_BUFFER_SIZE )
        {
            uart_buffer[uart_loc] = cThisChar;
            uart_loc++;

            //
            // Write the same character using the blocking write function.  This
            // function will not return until there was space in the FIFO and
            // the character is written.
            //
            ROM_UARTCharPut(UART0_BASE, cThisChar);
        }


    //
    // Stay in the loop until either a CR or LF is received.
    //
    } while((cThisChar != '\n') && (cThisChar != '\r'));

    return;
}

uint8_t hal_uart_buffer_get( char* buffer_out )
{
    uint8_t t_size;

    // We assume the consumer has allocated enough space for the whole buffer
    memcpy( uart_buffer, buffer_out, uart_loc );

    // "Clear" the buffer
    t_size = uart_loc;
    uart_loc = 0;

    return t_size;
}

void hal_uart_put( const char* output, uint32_t out_sz )
{
    uint32_t i;
    for( i=0; i<out_sz; i++ )
    {
        ROM_UARTCharPut(UART0_BASE, output[i]);
    }
    //ROM_UARTCharPut(UART0_BASE, '@');
}