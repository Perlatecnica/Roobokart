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

#include "ToF53L0A1.h"

#include "../../roobokart/roobokart_def.h"
#include "VL53L0X.h"

ToF53L0A1::ToF53L0A1(){
	/* It creates*/
	device_i2c = new DevI2C(I2C_SDA, I2C_SCL);

	/* creates the 53L0A1 expansion board singleton obj */
	board = XNucleo53L0A1::instance(device_i2c, NC, NC, NC); // NC connected interrupt pin

	/* Splash message*/
	board->display->display_string("CIAO");

	/* init the 53L0A1 expansion board with default values */
	status = board->init_board();

	if (status) {
		printf("Failed to init board!\n\r");
	}
	else{
		operating_mode = range_continuous_polling;
		board->sensor_left->start_measurement(operating_mode, NULL);
		board->sensor_right->start_measurement(operating_mode, NULL);
	}

}


uint16_t ToF53L0A1::getLeftMeasure(){
	board->sensor_left->get_measurement(operating_mode, &data_sensor_left);
#ifdef DEBUG_PROXIMITY
	printf("Proximity sensor left %d\r\n",data_sensor_left.RangeMilliMeter);
#endif
	return data_sensor_left.RangeMilliMeter;
}

uint16_t ToF53L0A1::getRightMeasure(){
	board->sensor_right->get_measurement(operating_mode, &data_sensor_right);
#ifdef DEBUG_PROXIMITY
	printf("Proximity sensor right %d\r\n",data_sensor_right.RangeMilliMeter);
#endif
	return data_sensor_right.RangeMilliMeter;
}

/* On board 4 digit local display refresh */
void ToF53L0A1::display_refresh(VL53L0X_RangingMeasurementData_t sensor_range_data)
{
    char str[5];
    if (sensor_range_data.RangeStatus == 0) {   // we have a valid range.
        sprintf(str, "%d", sensor_range_data.RangeMilliMeter);
    } else {
        sprintf(str, "%s", "----");
    }
    board->display->display_string(str);
}

/* It prints the int value on board 4 digit local display*/
void ToF53L0A1::display(int value){
	char str[5];
	sprintf(str, "%d", value);
	board->display->display_string(str);
}

/* It prints the int value on board 4 digit local display*/
void ToF53L0A1::display(char *value){
	board->display->display_string(value);
}


