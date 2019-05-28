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

#include "../roobokart_def.h"
#include "TCS3200.h"
#include "STSpin240_250.h"
#include "HTS221Sensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"
#include "MotorShieldIHM12A1.h"
#include "ToF53L0A1.h"
#include "PID.h"
#include "MEMS_IKS01A2.h"



class Devices {

public:
    Devices(Serial* ser);
    MotorShieldIHM12A1 currMotors;

    //Create a TCS3200 object S0    S1    S2     S3     OUT
    TCS3200 *color;

    ToF53L0A1 *tof;
    MEMS_IKS01A2 *mems;

    AnalogIn *rfrontIR;
    AnalogIn *lfrontIR;
    AnalogIn *cfrontIR;

    DigitalIn *usrButton;
    float roadsign_detect_threshold; // Roadsign detection threshold
    bool roadsignDetected(float cfrontIRvalue);

    void setSPDirection(float spd);
    float getSPDirection(void);
    void setRedThreshold(int redt);
    int getRedThreshold();
    int getGreenThreshold();
    void setGreenThreshold(int greent);

private:
    float setPointDirection;
    int redthreshold;
    int greenthreshold;
};

#endif

