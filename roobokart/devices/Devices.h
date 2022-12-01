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

#ifndef DEVICES_H
#define DEVICES_H

#include "../../perlatecnica/PLT_MEMS_IKS01A2/Gyro.h"
#include "../roobokart_def.h"
#include "TrafficLightsReader.h"
#include "STSpin240_250.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"
#include "MotorShieldIHM12A1.h"
#include "ToF53L0A1.h"
#include "PID.h"



class Devices {

public:
    Devices(Serial* ser);
    MotorShieldIHM12A1 currMotors;

    TrafficLightsReader *trafficLightReader;

    DevI2C * device_i2c;

#ifdef ROOBOKART_V3
    DevI2C * device_i2c3;
#endif

    ToF53L0A1 *tof;
    Gyro *mems;


private:
    float maxValueIR = 0;
    float minValueIR = 0;
#ifdef ROOBOKART_V3
    AnalogIn *rfrontIR_0;
    AnalogIn *rfrontIR_1;
    AnalogIn *rfrontIR_2;
#endif
    AnalogIn *rfrontIR;
    AnalogIn *lfrontIR;
    AnalogIn *cfrontIR;

    float readIR(AnalogIn*ir, bool raw = false)
    {
    	if (raw) return ir->read();
    	return map(ir->read(),minValueIR, maxValueIR, 0.0f, 1.0f);
    }

public:


    inline void setMaxIR(float value) {	maxValueIR = value; }
    inline void setMinIR(float value) {	minValueIR = value; }

    inline float readLeftIR(bool raw = false)
    {
    	return map(lfrontIR->read(),IR_L_MIN_VALUE, IR_L_MAX_VALUE, 0.0f, 1.0f);
    }

    inline float readCentreIR(bool raw = false)
    {
    	return map(cfrontIR->read(),IR_C_MIN_VALUE, IR_C_MAX_VALUE, 0.0f, 1.0f);
    }

    inline float readRightIR(bool raw = false)
    {
    	return map(rfrontIR->read(),IR_R_MIN_VALUE, IR_R_MAX_VALUE, 0.0f, 1.0f);
    }

    DigitalIn *usrButton;

    Serial *BT_hc05;

    static float map(float x, float in_min, float in_max, float out_min, float out_max)
	{
    	float value;
		value = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		if (value > out_max) return out_max;
		if (value < out_min) return out_min;
		return value;
	}

    bool roadsignDetected(float cfrontIRvalue);

    void setSPDirection(float spd);
    float getSPDirection(void);

    void buzz(int period_us, float dutycycle, int time);


private:
    float setPointDirection;
    int redthreshold;
    int greenthreshold;
    // Onboard buzzer
    PwmOut *Buzzer;
};


#endif

