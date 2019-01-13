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

#ifndef ROADSIGNALIGN_MODE_H
#define ROADSIGNALIGN_MODE_H

#include "../../../roobokart/devices/Devices.h"
#include "../../../roobokart/planning/Planning.h"
#include "../../roobokart_def.h"

#define NOT_ALIGNED_STATUS 0
#define READ_CODE_STATUS 1
#define PREPARE_NEXT_STATUS 2
#define BITS_TO_BE_READ 2


class RoadSignAlignMode {

public:
	RoadSignAlignMode(Serial* ser,Devices* devices,int yourmode, Planning *planning);
    int runMode(void);

private:
    Devices* currDevices;
    Planning *currPlanning;
    int currentmode;

    int mymode;
    int nextmode;
    float rfrontIR;
    float lfrontIR;
    float cfrontIR;
    int8_t direction;
};
#endif

