// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _RPM_H_
#define _RPM_H_
#include "Arduino.h"

#define REFRESH_INTERVAL 500
#define COMMON_ANODE true

#if defined (__AVR_ATtiny85__)
#define SERIAL_DEBUG false

#define DIN_PIN 2
#define CLK_PIN 1
#define CS_PIN 0

#define INTERRUPTPIN PCINT4
#define READPIN PINB4
#define SENSE_PIN 4
#else // arduino
#define SERIAL_DEBUG true

#define DIN_PIN 9
#define CLK_PIN 13
#define CS_PIN 8

#define SENSE_PIN 2

#endif

void detect();
void setAnodeDigit(uint8_t digit, uint8_t value, bool dot);

#endif /* _RPM_H_ */
