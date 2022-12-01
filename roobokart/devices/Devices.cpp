/* Copyright (c) 2019 Perlatecnica no-profit organization
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../../roobokart/devices/Devices.h"

Devices::Devices(Serial* ser) {
#ifdef ROOBOKART_V3
	ser->printf("\r\n\tROOBOKART v3.0.0\r\n");
#else
	ser->printf("\r\n\tROOBOKART v2.1.0\r\n");
#endif

	// IR sensors instances
	#ifdef ROOBOKART_V3
	rfrontIR_0 = new AnalogIn(RIGHT_FRONT_0_IR);
	rfrontIR_1 = new AnalogIn(RIGHT_FRONT_1_IR);
	rfrontIR_2 = new AnalogIn(RIGHT_FRONT_2_IR);
	rfrontIR = rfrontIR_2;
	#else
	rfrontIR = new AnalogIn(RIGHT_FRONT_IR);
	#endif
	lfrontIR = new AnalogIn(LEFT_FRONT_IR);
	cfrontIR = new AnalogIn(CENTRE_FRONT_IR);

	// User button instance
	usrButton = new DigitalIn(USER_BUTTON);



	// I2C Port
	device_i2c = new DevI2C(I2C_SDA, I2C_SCL);

	#ifdef ROOBOKART_V3
	// I2C Port 3
	device_i2c3 = new DevI2C(I2C3_SDA, I2C3_SCL);

	// Proximity sensors instance
	tof = new ToF53L0A1(device_i2c3, SPI3_MOSI, SPI3_MISO, SPI3_SCK, SHIFT_REG_LE, DISPLAY_CS, DISPLAY_A0, DISPLAY_LED);

	// Color sensor instance
	trafficLightReader = new TrafficLightsReader(device_i2c3);

	#else

	// Proximity sensors instance
	tof = new ToF53L0A1(device_i2c);

	// Color sensor instance
	trafficLightReader = new TrafficLightsReader();

	#endif

	// MEMS sensors instance
	mems = new Gyro(device_i2c);

	Buzzer = new PwmOut(BUZZER_PIN);

	// Bluetooth HC05 Connection
	BT_hc05 = new Serial(BT_TX,BT_RX);

	device_i2c->frequency(1000000);
}


bool Devices::roadsignDetected(float cfrontIRvalue){
	return (cfrontIRvalue < ROADSIGN_DETECTION_THRESHOLD_DEFAULT);
}

void Devices::setSPDirection(float spd){
	setPointDirection = spd;
}

float Devices::getSPDirection(){
	return setPointDirection;
}

void Devices::buzz(int period_us, float dutycycle, int time){
	Buzzer->period_us(7633);
	Buzzer->write(0.50f);
	wait(time);
	Buzzer->write(0.0f);
}
