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


#ifdef ROOBOKART_V3

ToF53L0A1::ToF53L0A1(DevI2C *dev_i2c, PinName mosi, PinName miso, PinName sck, PinName sr_cs,
		PinName display_cs, PinName display_a0, PinName display_led)
: device_i2c(dev_i2c), sr_dev(mosi, miso, sck, sr_cs),
  sr_left_cs(&sr_dev,SR74HC595::GPIO_4),
  sr_right_cs(&sr_dev,SR74HC595::GPIO_6),
  sr_centre_cs(&sr_dev,SR74HC595::GPIO_5),
  display_lcd(mosi, miso, sck, display_cs, display_a0, NC),
  display_reset_pin(&sr_dev,SR74HC595::GPIO_7),
  display_led_pin(display_led,0),
  tof_center(dev_i2c,&timer1), tof_left(dev_i2c,&timer2), tof_right(dev_i2c,&timer3)
{
	sr_dev.setAll(0b00000000);
	ThisThread::sleep_for(100);

	/* DISPLAY */
	display_reset_pin.write(1);
	display_led_pin.write(0);
	display_lcd.initR(ST7735::InitR::BlackTab18);
	display_lcd.fillScreen(ST7735::Black);
	display_lcd.setRotation(3);
	display_lcd.setTextSize(2);

	display_lcd.setCursor(0,0);
	display_lcd.setTextColor(ST7735::Blue, ST7735::Black);
	display_lcd.printf("Perlatecnica\r\n\r\nRoobokart V3\r\n");

	/* TOF CENTRE */
	sr_centre_cs.write(1);
	ThisThread::sleep_for(100);
	tof_center.setTimeout(500);
	if(tof_center.init())
	{
		printf("Sensor centre detected\r\n");
		tof_center.setDistanceMode(VL53L1X::Short);
		tof_center.setMeasurementTimingBudget(50000);
		tof_center.setAddress(0x54);
		tof_center.startContinuous(200);
		centreD = true;
	}
	else
	{
		printf("Sensor centre not present\r\n");
		centreD = false;
	}

	/* TOF LEFT */
	sr_left_cs.write(1);
	ThisThread::sleep_for(100);
	tof_left.setTimeout(500);
	if(tof_left.init())
	{
		printf("Sensor left detected\r\n");
		tof_left.setDistanceMode(VL53L1X::Short);
		tof_left.setMeasurementTimingBudget(50000);
		tof_left.setAddress(0x56);
		tof_left.startContinuous(200);
		leftD = true;
	}
	else
	{
		printf("Sensor left not present\r\n");
		leftD = false;
	}

	/* TOF RIGHT */
	sr_right_cs.write(1);
	ThisThread::sleep_for(100);
	tof_right.setTimeout(500);
	if(tof_right.init())
	{
		printf("Sensor right detected\r\n");
		tof_right.setDistanceMode(VL53L1X::Short);
		tof_right.setMeasurementTimingBudget(50000);
		tof_right.setAddress(0x58);
		tof_right.startContinuous(200);
		rightD = true;
	}
	else
	{
		printf("Sensor right not present\r\n");
		rightD = false;
	}

	display_lcd.fillScreen(ST7735::Black);
	display_lcd.setTextSize(5);
}

uint16_t ToF53L0A1::getCentreMeasure()
{
	tof_center.read();
	switch (tof_center.ranging_data.range_status)
	{
	case VL53L1X::RangeValid:
	case VL53L1X::None:
		return tof_center.ranging_data.range_mm;
	default:
#ifdef DEBUG_PROXIMITY
		printf("Center sensor status: %s\r\n",VL53L1X::rangeStatusToString(tof_center.ranging_data.range_status));
#endif
		return USHRT_MAX;
	}
}

uint16_t ToF53L0A1::getRightMeasure()
{
	tof_right.read();
	switch (tof_right.ranging_data.range_status)
	{
	case VL53L1X::RangeValid:
	case VL53L1X::None:
		return tof_right.ranging_data.range_mm;
	default:
#ifdef DEBUG_PROXIMITY
		printf(" Right sensor status: %s\r\n",VL53L1X::rangeStatusToString(tof_right.ranging_data.range_status));
#endif
		return USHRT_MAX;
	}
}

uint16_t ToF53L0A1::getLeftMeasure()
{
	tof_left.read();
	switch (tof_left.ranging_data.range_status)
	{
	case VL53L1X::RangeValid:
	case VL53L1X::None:
		return tof_left.ranging_data.range_mm;
	default:
#ifdef DEBUG_PROXIMITY
		printf("  Left sensor status: %s\r\n",VL53L1X::rangeStatusToString(tof_left.ranging_data.range_status));
#endif
		return USHRT_MAX;
	}
}

/* It prints the int value on board 4 digit local display*/
void ToF53L0A1::display(int value){
	display_lcd.setCursor(0,0);
	display_lcd.setTextColor(ST7735::Blue, ST7735::Black);
	display_lcd.printf("%4d",value);
}

/* It prints the int value on board 4 digit local display*/
void ToF53L0A1::display(char *value){
	display_lcd.setCursor(0,0);
	display_lcd.setTextColor(ST7735::Blue, ST7735::Black);
	display_lcd.printf("%4s",value);
}

#else

ToF53L0A1::ToF53L0A1(DevI2C *dev_i2c) : device_i2c(dev_i2c) {
	/* It creates*/
	//device_i2c = new DevI2C(I2C_SDA, I2C_SCL);

	/* creates the 53L0A1 expansion board singleton obj */
	board = XNucleo53L0A1::instance(device_i2c, NC, NC, NC); // NC connected interrupt pin

	/* Splash message*/
	board->display->display_string("CIAO");

	/* init the 53L0A1 expansion board with default values */
	status = board->init_board();

	leftD = board->sensor_left != nullptr;
	rightD = board->sensor_right != nullptr;
	centreD = board->sensor_centre != nullptr;

	if (status) {
		printf("Failed to init board!\n\r");
	}
	else{
		operating_mode = range_continuous_polling;
		if (board->sensor_left) board->sensor_left->start_measurement(operating_mode, NULL);
		if (board->sensor_right) board->sensor_right->start_measurement(operating_mode, NULL);
	}

}


uint16_t ToF53L0A1::getLeftMeasure(){
	if (board->sensor_left){
	board->sensor_left->get_measurement(operating_mode, &data_sensor_left);
#ifdef DEBUG_PROXIMITY
	printf("Proximity sensor left %d\r\n",data_sensor_left.RangeMilliMeter);
#endif
	if (data_sensor_left.RangeMilliMeter)
		return data_sensor_left.RangeMilliMeter;
	}
	return 8190;
}

uint16_t ToF53L0A1::getRightMeasure(){
	if (board->sensor_right){
	board->sensor_right->get_measurement(operating_mode, &data_sensor_right);
#ifdef DEBUG_PROXIMITY
	printf("Proximity sensor right %d\r\n",data_sensor_right.RangeMilliMeter);
#endif
	if (data_sensor_right.RangeMilliMeter)
			return data_sensor_right.RangeMilliMeter;
	}
	return 8190;
}

uint16_t ToF53L0A1::getCentreMeasure(){
		return 0;
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

#endif
