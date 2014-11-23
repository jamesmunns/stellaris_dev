#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#include "shiftbrite.h"

#include "utl_shifty.h"

#define LED_RED     GPIO_PIN_1
#define LED_BLUE    GPIO_PIN_2
#define LED_GREEN   GPIO_PIN_3

#define DATA_PIN    GPIO_PIN_4
#define LATCH_PIN   GPIO_PIN_5
#define ENABLE_PIN  GPIO_PIN_6
#define CLOCK_PIN   GPIO_PIN_7

/*
RS  - PD3
EN  - PE1
DB7 - PE2
DB6 - PE3
DB5 - PF1
DB4 - PE5
*/

shiftbrite sb;

int main()
{
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

  //LEDs
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN);

  //Shifty
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, LATCH_PIN|ENABLE_PIN|CLOCK_PIN);

  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, DATA_PIN);

  port_pin D,L,E,C;

  D.port = GPIO_PORTB_BASE;
  D.pin  = DATA_PIN;

  L.port = GPIO_PORTA_BASE;
  L.pin  = LATCH_PIN;

  E.port = GPIO_PORTA_BASE;
  E.pin  = ENABLE_PIN;

  C.port = GPIO_PORTA_BASE;
  C.pin  = CLOCK_PIN;

  shiftbrite_init(&sb,
                  D,  // Data pin   : 1.4
                  L,  // Latch pin  : 1.5
                  E,  // Enable pin : 1.6
                  C ); // Clock pin  : 1.7

  //for( int i = 0; i<5; i++ )
  //{
      shiftbrite_configure( &sb, 120, 100, 100, CLOCK_800MHZ );
  //}

  // At startup, the module is disabled. Enable it.
  shiftbrite_enable(&sb);
  shiftbrite_latch(&sb);

  for (;;) {

    // set the red LED pin high, others low
    //ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED|LED_BLUE);

    for( int i = 0; i < 128; i++ )
    {
      shiftbrite_rgb( &sb, i, 0, 0 );
      shiftbrite_latch( &sb );
      ROM_SysCtlDelay(100000);
    }

    for( int i = 127; i >= 0; i-- )
    {
      shiftbrite_rgb( &sb, i, 0, 0 );
      shiftbrite_latch( &sb );
      ROM_SysCtlDelay(100000);
    }

  ROM_SysCtlDelay(5000000);

  }

}
