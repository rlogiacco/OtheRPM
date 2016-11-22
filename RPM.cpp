// Do not remove the include below
#include "RPM.h"

#include <LedControl.h>
#include <elapsedMillis.h>
#include <FormattingSerialDebug.h>

//#define NUM_DIGITS 4
//#define DIGIT_1_PIN 1
//#define DIGIT_2_PIN 2
//#define DIGIT_3_PIN 3
//#define DIGIT_4_PIN 4
//
//#define SEG_A_PIN 1
//#define SEG_B_PIN 2
//#define SEG_C_PIN 3
//#define SEG_D_PIN 4
//#define SEG_E_PIN 5
//#define SEG_F_PIN 6
//#define SEG_G_PIN 7
//#define SEG_DP_PIN 8



LedControl ledControl = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

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

//SevSeg display;
elapsedMillis ms;

void setup() {
	SERIAL_DEBUG_SETUP(9600);
	pinMode(SENSE_PIN, INPUT_PULLUP);
	attachInterrupt(INT, detect, FALLING);

	ledControl.shutdown(0, false);
	ledControl.setIntensity(0, 15);
	ledControl.clearDisplay(0);
}

volatile unsigned long counter = 0;

void loop() {
	if (ms >= REFRESH_INTERVAL) {
		unsigned long rpm = (60000 / ms) * counter;
		DEBUG("CNT %lu in %lums = RPM %lu", counter, (unsigned long)ms, rpm);
		counter = 0;
		ms = 0;
		display(rpm);
	}
}

void detect() {
	counter++;
}

const char digits[] = { (char)0b11111100, //0
						(char)0b01100000, //1
						(char)0b11011010, //2
						(char)0b11110010, //3
						(char)0b01100110, //4
						(char)0b10110110, //5
						(char)0b10111110, //6
						(char)0b11100000, //7
						(char)0b11111110, //8
						(char)0b11110110  //9
					  };

void setAnodeDigit(uint8_t digit, uint8_t value, bool dot) {
	char _value = dot ? digits[value] | 0b00000001 : digits[value];
	ledControl.setColumn(0, digit, _value);
}
