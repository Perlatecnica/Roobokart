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

#include "XNucleo53L0A1.h"

class ToF53L0A1 {

public:
	ToF53L0A1();
	void display_refresh(VL53L0X_RangingMeasurementData_t sensor_range_data);
	XNucleo53L0A1 *board;
	void display(int value);
	void display(char *value);

private:
	DevI2C *device_i2c;
	VL53L0X_RangingMeasurementData_t data_sensor_centre;
	int status;
};
#endif

