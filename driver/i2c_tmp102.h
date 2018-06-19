#ifndef _I2C_TMP102_H
#define _I2C_TMP102_H

#include <stdbool.h>
#include "i2c_eeprom.h"

#define TEMPERATURE_REGISTER 	0x00
#define CONFIG_REGISTER 			0x01
#define T_LOW_REGISTER 				0x02
#define T_HIGH_REGISTER 			0x03

#define TMP102_ADDRESS	0x48


//void tmp102_openPointerRegister(uint8_t pointerReg);
//uint8_t tmp102_readRegister(bool registerNumber);

// Returns the temperature in degrees C
float tmp102_readTempC(void);

// Converts readTempC result to degrees F
float tmp102_readTempF(void);

// Set the conversion rate (0-3)
		// 0 - 0.25 Hz
		// 1 - 1 Hz
		// 2 - 4 Hz (default)
		// 3 - 8 Hz
void tmp102_setConversionRate(uint8_t rate);

// Enable or disable extended mode
		// 0 - disabled (-55C to +128C)
		// 1 - enabled  (-55C to +150C)
void tmp102_setExtendedMode(bool mode);

// Switch sensor to low power mode
void tmp102_sleep(void);

// Wakeup and start running in normal power mode
void tmp102_wakeup(void);

// set One-Shot
void tmp102_oneShot(void);

#endif
