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

Devices::Devices(Serial* ser){
	rfrontIR = new AnalogIn(RIGHT_FRONT_IR);
	lfrontIR = new AnalogIn(LEFT_FRONT_IR);
	cfrontIR = new AnalogIn(CENTRE_FRONT_IR);

	usrButton = new DigitalIn(USER_BUTTON);

	color = new TCS3200(TCS3200_S0, TCS3200_S1, TCS3200_S2, TCS3200_S3, TCS3200_OUT);

	tof = new ToF53L0A1();

	mems = new MEMS_IKS01A2();
	mems->init(IKS01A2_I2C_SDA, IKS01A2_I2C_SCL, IKS01A2_INT1, IKS01A2_INT2);
	mems->setODR(155);

	roadsign_detect_threshold = ROADSIGN_DETECTION_THRESHOLD_DEFAULT;
}


bool Devices::roadsignDetected(float cfrontIRvalue){
	return (cfrontIRvalue < roadsign_detect_threshold);
}

void Devices::setSPDirection(float spd){
	setPointDirection = spd;
}

float Devices::getSPDirection(){
	return setPointDirection;
}

void Devices::setRedThreshold(int redt){
	float temp = redt*20;
	temp = temp / 100;
	redthreshold = redt - temp;
}

int Devices::getRedThreshold(){
	return redthreshold;
}

void Devices::setGreenThreshold(int greent){
	float temp = greent*10;//20;
	temp = temp / 100;
	greenthreshold = greent - temp;
}

int Devices::getGreenThreshold(){
	return greenthreshold;
}
