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

#ifndef NAV_MODE_H
#define NAV_MODE_H

#include "../../../roobokart/devices/Devices.h"
#include "../../../roobokart/planning/Planning.h"
#include "../../roobokart_def.h"

#define NORMAL_NAV_STATUS 0
#define HARD_CURVE_STATUS 1

class NavMode {

public:
	NavMode(Serial* ser,Devices* devices,int yourmode, Planning *planning);
    int runMode(void);

private:
    Devices* currDevices;
    int currentmode;
    int nextmode;
    int mymode;
    float rfrontIR;
    float lfrontIR;
    float cfrontIR;
    float currentdirection;
    float spd;
    int8_t direction;
    PID *dirPID;
    Timer pidtimer;
    Planning *currPlanning;
    int navstatus;
};
#endif

