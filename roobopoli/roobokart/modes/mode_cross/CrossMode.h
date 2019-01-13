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

#ifndef CROSS_MODE_H
#define CROSS_MODE_H

#include "../../../roobokart/devices/Devices.h"
#include "../../../roobokart/planning/Planning.h"
#include "../../roobokart_def.h"

class CrossMode {

public:
    CrossMode(Serial* ser,Devices* devices, int yourmode, Planning *planning);
    int runMode(void);

private:
    Devices* currDevices;
    Planning *currPlanning;
    int currentmode;
    int nextmode;
    int mymode;
    int currentDirection;
    float currentYaw;
    float setPointYaw;
    int8_t direction;
    PID *dirPID;
    Timer pidtimer;
    Timer crosstimer;
    float rfrontIR;
    float lfrontIR;
    float cfrontIR;
  };
#endif

