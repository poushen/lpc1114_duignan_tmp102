#include "i2c_tmp102.h"
//#include "interrupt_handler_utility.h"
#include "systick.h"

//void tmp102_openPointerRegister(uint8_t pointerReg)
//{ 
//  // Open specified register
//	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
//	i2c_eeprom_write_address(TMP102_ADDRESS, pointerReg);
//}

//uint8_t tmp102_readRegister(bool registerNumber)
//{  
//  uint8_t registerByte[2];	// We'll store the data from the registers here
//  
//  // Read current configuration register value
//  // Read two bytes from TMP102
//	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
//	i2c_eeprom_sequential_read(TMP102_ADDRESS, registerByte, 2);
//  
//  return registerByte[registerNumber];
//}

float tmp102_readTempC(void)
{
  uint8_t registerByte[2];	// Store the data from the register here
  int16_t digitalTemp;  // Temperature stored in TMP102 register
  
  // Read Temperature
  // Change pointer address to temperature register (0)
//  tmp102_openPointerRegister(TEMPERATURE_REGISTER);
  // Read from temperature register
//  registerByte[0] = tmp102_readRegister(0);
//  registerByte[1] = tmp102_readRegister(1);
	
	i2c_eeprom_random_read(TMP102_ADDRESS, TEMPERATURE_REGISTER, registerByte, 2);

  // Bit 0 of second byte will always be 0 in 12-bit readings and 1 in 13-bit
  if(registerByte[1]&0x01)	// 13 bit mode
  {
		// Combine bytes to create a signed int
    digitalTemp = ((registerByte[0]) << 5) | (registerByte[1] >> 3);
		// Temperature data can be + or -, if it should be negative,
		// convert 13 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0xFFF)
		{
      digitalTemp |= 0xE000;
    }
  }
  else	// 12 bit mode
  {
		// Combine bytes to create a signed int 
    digitalTemp = ((registerByte[0]) << 4) | (registerByte[1] >> 4);
		// Temperature data can be + or -, if it should be negative,
		// convert 12 bit to 16 bit and use the 2s compliment.
    if(digitalTemp > 0x7FF)
		{
      digitalTemp |= 0xF000;
    }
  }
  // Convert digital reading to analog temperature (1-bit is equal to 0.0625 C)
  return digitalTemp*0.0625;
}

float tmp102_readTempF(void)
{
	return tmp102_readTempC()*9.0/5.0 + 32.0;
}

void tmp102_setConversionRate(uint8_t rate)
{
  uint8_t registerByte[2]; // Store the data from the register here
  rate = rate&0x03; // Make sure rate is not set higher than 3.
  
	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
  // Change pointer address to configuration register (0x01)
//  tmp102_openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
//  registerByte[0] = tmp102_readRegister(0);
//  registerByte[1] = tmp102_readRegister(1);
//	i2c_eeprom_sequential_read(TMP102_ADDRESS, registerByte, 2);

	i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, registerByte, 2);
  // Load new conversion rate
  registerByte[1] &= 0x3F;  // Clear CR0/1 (bit 6 and 7 of second byte)
  registerByte[1] |= rate<<6;	// Shift in new conversion rate

  // Set configuration registers
	i2c_eeprom_page_write(TMP102_ADDRESS, CONFIG_REGISTER, registerByte, 2);

}

void tmp102_setExtendedMode(bool mode) 
{
  uint8_t registerByte[2]; // Store the data from the register here

	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
  // Change pointer address to configuration register (0x01)
//  tmp102_openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
//  registerByte[0] = tmp102_readRegister(0);
//  registerByte[1] = tmp102_readRegister(1);
//	i2c_eeprom_sequential_read(TMP102_ADDRESS, registerByte, 2);

	i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, registerByte, 2);
  // Load new value for extention mode
  registerByte[1] &= 0xEF;		// Clear EM (bit 4 of second byte)
  registerByte[1] |= mode<<4;	// Shift in new exentended mode bit

  // Set configuration registers
	i2c_eeprom_page_write(TMP102_ADDRESS, CONFIG_REGISTER, registerByte, 2);
}

void tmp102_sleep(void)
{
  uint8_t registerByte; // Store the data from the register here

	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
  // Change pointer address to configuration register (0x01)
//  tmp102_openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  //registerByte = tmp102_readRegister(0);
//	i2c_eeprom_current_read(TMP102_ADDRESS, &registerByte);

	i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, &registerByte, 1);
  registerByte |= 0x01;	// Set SD (bit 0 of first byte)

  // Set configuration register
	i2c_eeprom_byte_write(TMP102_ADDRESS, CONFIG_REGISTER, registerByte);
}

void tmp102_wakeup(void)
{
  uint8_t registerByte; // Store the data from the register here

	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
  // Change pointer address to configuration register (1)
//  tmp102_openPointerRegister(CONFIG_REGISTER);
  
  // Read current configuration register value
  //registerByte = tmp102_readRegister(0);
//	i2c_eeprom_current_read(TMP102_ADDRESS, &registerByte);

	i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, &registerByte, 1);
  registerByte &= 0xFE;	// Clear SD (bit 0 of first byte)

  // Set configuration registers
	i2c_eeprom_byte_write(TMP102_ADDRESS, CONFIG_REGISTER, registerByte);
}

void tmp102_oneShot(void)
{
	uint8_t registerByte; // Store the data from the register here

	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
	i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, &registerByte, 1);
  registerByte |= 0x80;	// Set OS (bit 7 of first byte)

  // Set configuration registers
	i2c_eeprom_byte_write(TMP102_ADDRESS, CONFIG_REGISTER, registerByte);
	
	do {
		delay_ms(26);
		i2c_eeprom_random_read(TMP102_ADDRESS, CONFIG_REGISTER, &registerByte, 1);
		registerByte &= 0x80;
	} while (registerByte == 0);
}
