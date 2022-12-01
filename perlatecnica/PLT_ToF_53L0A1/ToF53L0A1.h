/* Copyright (c) 2016-2017 perlatecnica.it, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TOF_SHIELD53L0A1_H
#define TOF_SHIELD53L0A1_H

#ifdef ROOBOKART_V3
	#include "74HC595.h"
	#include "DevI2C.h"
	#include "VL53L1X.h"
	#include "ST7735.h"
#else
	#include "XNucleo53L0A1.h"
#endif

class ToF53L0A1 {

public:
	ToF53L0A1() = delete;

	#ifdef ROOBOKART_V3
	ToF53L0A1(DevI2C *dev_i2c, PinName mosi, PinName miso, PinName sck, PinName sr_cs,
			PinName display_cs, PinName display_a0, PinName display_led);

	private:
	Timer timer1;
	Timer timer2;
	Timer timer3;

	VL53L1X tof_center;
	VL53L1X tof_left;
	VL53L1X tof_right;


	#else
	ToF53L0A1(DevI2C *dev_i2c);
	void display_refresh(VL53L0X_RangingMeasurementData_t sensor_range_data);
	XNucleo53L0A1 *board;
	#endif

public:
	void display(int value);
	void display(char *value);
	uint16_t getLeftMeasure();
	uint16_t getRightMeasure();
	uint16_t getCentreMeasure();

	bool isLeftDetected() { return leftD; }
	bool isRightDetected() { return rightD; }
	bool isCentreDetected() { return centreD; }


private:

	bool leftD,rightD,centreD;

	DevI2C *device_i2c;

	#ifdef ROOBOKART_V3
	SR74HC595 sr_dev;
	SR74HC595DigitalOut sr_left_cs;
	SR74HC595DigitalOut sr_right_cs;
	SR74HC595DigitalOut sr_centre_cs;
	SR74HC595DigitalOut display_reset_pin;
	DigitalOut display_led_pin;
	ST7735 display_lcd;

	#else
	VL53L0X_RangingMeasurementData_t data_sensor_left;
	VL53L0X_RangingMeasurementData_t data_sensor_right;
	OperatingMode operating_mode;// = range_single_shot_polling;
	int status;
	#endif
};
#endif

