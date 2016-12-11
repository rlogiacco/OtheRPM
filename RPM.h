// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _RPM_H_
#define _RPM_H_
#include "Arduino.h"

// how often the display is updated
#define REFRESH_INTERVAL 500
// display type
#define COMMON_ANODE true

#define MONITOR_BATTERY false
// BATTERY ONLY
// input voltage
#define VIN 5.0
// low battery voltage
#define BATT_LOW 3.4
// voltage sensing divider ratio
#define R1 10.0
#define R2 20.0
#define BATT_LOW_BLINK_INTERVAL 250



#if defined (__AVR_ATtiny85__)
#define SERIAL_DEBUG false

#define DIN_PIN 2
#define CLK_PIN 1
#define CS_PIN 0

#define INTERRUPTPIN PCINT3
#define READPIN PINB3
#define INT_PIN 3
#define SENSE_PIN 4

#else // arduino
#define SERIAL_DEBUG true

#define DIN_PIN 9
#define CLK_PIN 13
#define CS_PIN 8

#define INT_PIN 2
#define SENSE_PIN A0
#endif

void detect();
void setAnodeDigit(uint8_t digit, uint8_t value, bool dot);

#endif /* _RPM_H_ */
