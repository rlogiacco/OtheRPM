// Do not remove the include below
#include "RPM.h"
#include "CircularBuffer.h"

#include <LedControl.h>
#include <elapsedMillis.h>
#include <FormattingSerialDebug.h>

LedControl ledControl = LedControl(DIN_PIN, CLK_PIN, CS_PIN);

void display(unsigned long num) {
#if (COMMON_ANODE)
	setAnodeDigit(4, num / 10 % 10, false);
	setAnodeDigit(3, num / 100 % 10, false);
	setAnodeDigit(2, num / 1000 % 10, true);
	if (num / 10000 > 0) {
		setAnodeDigit(1, num / 10000 % 10, false);
	} else {
		ledControl.setColumn(0, 1, 0b00000000);
	}
#else
	ledControl.setDigit(0, 4 ,num / 10 % 10, false);
	ledControl.setDigit(0, 3 ,num / 100 % 10, false);
	ledControl.setDigit(0, 2 ,num / 1000 % 10, true);
	if (num / 10000 > 0) {
		ledControl.setDigit(0, 1 ,num / 10000 % 10, false);
	} else {
		ledControl.setRow(0, 1, 0b00000000);
	}
#endif
}

elapsedMillis ms;
void setup() {
	cli();
	SERIAL_DEBUG_SETUP(9600);
	pinMode(INT_PIN, INPUT_PULLUP);

#ifdef __AVR_ATtiny85__
	PCMSK |= (1 << INTERRUPTPIN); // set pin change mask to listen to pin state changes
	GIMSK |= (1 << PCIE);   // enable PCINT interrupt in the general interrupt mask
#else //arduino
	attachInterrupt(digitalPinToInterrupt(INT_PIN), detect, FALLING);
#endif

	ledControl.shutdown(0, false);
	ledControl.setIntensity(0, 15);
	ledControl.clearDisplay(0);
#if (!COMMON_ANODE)
	ledControl.setScanLimit(0, 3);
#endif
	sei();

	pinMode(SENSE_PIN, INPUT);
}

volatile unsigned long counter = 0;
unsigned int battery_warning = 0;

void loop() {
	if (battery_warning > 0 && ms >= (BATT_LOW_BLINK_INTERVAL * battery_warning)) {
		ledControl.setIntensity(0, ((battery_warning++ % 2) * 7) + 8);
	}
	if (ms >= REFRESH_INTERVAL) {
		unsigned long rpm = (60000 / ms) * counter;
		DEBUG("CNT %lu in %lums = RPM %lu", counter, (unsigned long )ms, rpm);
		counter = 0;
		ms = 0;

		display(rpm);
#if (MONITOR_BATTERY)
		float voltage = analogRead(SENSE_PIN) * VIN / 1023 * ((R1 + R2) / R2);
		if (battery_warning == 0 && voltage < BATT_LOW) {
			battery_warning = 1;
		}
#endif
	}
}

void detect() {
	counter++;
}

#if defined (__AVR_ATtiny85__)
ISR(PCINT0_vect) {
	byte pinState = (PINB >> READPIN) & 1;
	if (pinState == 0) {
		detect();
	}
}
#endif

const char digits[] = { (char) 0b11111100, //0
		(char) 0b01100000, //1
		(char) 0b11011010, //2
		(char) 0b11110010, //3
		(char) 0b01100110, //4
		(char) 0b10110110, //5
		(char) 0b10111110, //6
		(char) 0b11100000, //7
		(char) 0b11111110, //8
		(char) 0b11110110  //9
		};

void setAnodeDigit(uint8_t digit, uint8_t value, bool dot) {
	char _value = dot ? digits[value] | 0b00000001 : digits[value];
	ledControl.setColumn(0, digit, _value);
}
