
#define HAL_UART_BUFFER_SIZE ( 64 )

#include "ajm_types.h"

void    hal_uart_init( void );
void    hal_uart_periodic( void );
uint8_t hal_uart_buffer_get( char* );
void    hal_uart_put( const char*, uint32_t );