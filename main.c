//
// main.c - a example for ARM LPC1114
// Written by Ted Burke (based on code by Frank Duignan)
// Last updated 29-11-2013
//

#include "lpc111x.h"
#include <stdint.h>
#include "led.h"
#include "uart.h"
#include "systick.h"
#include "i2c_eeprom.h"
#include "i2c_oled.h"
#include "i2c_tmp102.h"

#define BUFFER_SIZE 64

int main()
{
	float temperature;
  
	ledCfg();
	uartCfg();
	i2cConfig();
	SysTickCfg(48000); // let SysTick interrupt at 1MHz

	ledOn();
	uartPuts("\n*************\n");
	uartPuts("*  I2C Test *\n");
	uartPuts("*************\n");

	ADDRESS_SIZE = ONE_BYTE_ADDRESS;
	init_oled();

  //tmp102_setConversionRate(2);
	tmp102_setExtendedMode(0);
	tmp102_sleep();

	while (1) {
		ledOn();

		tmp102_oneShot();
		temperature = tmp102_readTempC();

		oled_setFont(BigNumbers);
		oled_clrScr();
		//oled_printNumI((long)temperature, 20, 10, 0, ' ');
		oled_printNumF(temperature, 2, 0, 0, '.', 0, ' ');
		oled_setFont(SmallFont);
		oled_print("~c", 60, 30);
		oled_update();

		ledOff();
		delay_ms(1000);
	}

}
