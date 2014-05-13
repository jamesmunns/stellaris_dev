/**
 * Protocol defined in Allegro A6281 datasheet:
 * http://www.allegromicro.com/en/Products/Part_Numbers/6281/6281.pdf
 *
 * See shiftbrite.h for API docs.
 *
 * 2011, Joe Walnes <joe@walnes.com>
 *
 * 2014, James Munns port to stellaris launchpad
 */

#include "shiftbrite.h"
#include "driverlib/rom.h"

static inline void pulse(port_pin pin);
static inline void shift_bit(shiftbrite* sb, uint16_t val);
static inline void shift_zeros(shiftbrite* sb, uint8_t count);
static inline void shift_10bit_value(shiftbrite* sb, uint16_t value);
static inline void shift_7bit_value(shiftbrite* sb, uint8_t value);

void shiftbrite_init(shiftbrite *sb,
                     port_pin data_pin,
                     port_pin latch_pin,
                     port_pin enable_pin,
                     port_pin clock_pin) {
  sb->data_pin   = data_pin;
  sb->latch_pin  = latch_pin;
  sb->enable_pin = enable_pin;
  sb->clock_pin  = clock_pin;
}

void shiftbrite_configure(shiftbrite *sb,
                          uint8_t red_correction,
                          uint8_t green_correction,
                          uint8_t blue_correction,
                          shiftbrite_clock clock) {
  shift_zeros(sb, 5);
  shift_7bit_value(sb, blue_correction);
  shift_zeros(sb, 3);
  shift_7bit_value(sb, red_correction);
  shift_zeros(sb, 1);
  switch(clock) {
    case CLOCK_200MHZ:
      shift_bit(sb, 1);
      shift_bit(sb, 1);
      break;
    case CLOCK_EXTERNAL:
      shift_bit(sb, 1);
      shift_bit(sb, 0);
      break;
    case CLOCK_400MHZ:
      shift_bit(sb, 0);
      shift_bit(sb, 1);
      break;
    case CLOCK_800MHZ:
    default:
      shift_bit(sb, 0);
      shift_bit(sb, 0);
      break;
  }
  shift_7bit_value(sb, green_correction);
}

void shiftbrite_enable(shiftbrite* sb) {
  ROM_GPIOPinWrite( sb->enable_pin.port, sb->enable_pin.pin, ~sb->enable_pin.pin );
}

void shiftbrite_disable(shiftbrite* sb) {
  ROM_GPIOPinWrite( sb->enable_pin.port, sb->enable_pin.pin, sb->enable_pin.pin );
}

void shiftbrite_rgb(shiftbrite* sb,
                    uint16_t red,
                    uint16_t green,
                    uint16_t blue) {
  shift_zeros(sb, 2);
  shift_10bit_value(sb, blue);
  shift_10bit_value(sb, red);
  shift_10bit_value(sb, green);
}

void shiftbrite_latch(shiftbrite* sb) {
  pulse(sb->latch_pin);
}

// -- Helpers --

static inline void pulse(port_pin pin) {
    ROM_GPIOPinWrite( pin.port, pin.pin, pin.pin );
    ROM_GPIOPinWrite( pin.port, pin.pin, ~pin.pin );
}

static inline void shift_bit(shiftbrite* sb, uint16_t val) {
  if( val )
  {
      ROM_GPIOPinWrite( sb->data_pin.port, sb->data_pin.pin, sb->data_pin.pin );
  }
  else
  {
      ROM_GPIOPinWrite( sb->data_pin.port, sb->data_pin.pin, ~sb->data_pin.pin );
  }
  pulse(sb->clock_pin);
}

static inline void shift_zeros(shiftbrite* sb, uint8_t count) {
  int i;
  for (i = 0; i < count; i++)  {
    shift_bit(sb, 0);
  }
}

static inline void shift_10bit_value(shiftbrite* sb, uint16_t value) {
  int i;
  for (i = 9; i >= 0; i--) {
    shift_bit(sb, value & (1 << i));
  }
}

static inline void shift_7bit_value(shiftbrite* sb, uint8_t value) {
  int i;
  for (i = 6; i >= 0; i--) {
    shift_bit(sb, value & (1 << i));
  }
}
