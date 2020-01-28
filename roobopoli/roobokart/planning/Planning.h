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

#ifndef PLANNING_H
#define PLANNING_H

#include "../../roobokart/devices/Devices.h"
#include "../roobokart_def.h"

#define MAX_NAV_SPEED 100
#define CRUISE_NAV_SPEED 50// Blue 35
#define MIN_NAV_SPEED 20

class Planning {

public:
	Planning(Devices* devices);
	void setCrossCodeRead(uint32_t coderead);
	uint32_t getCrossCode();
	int GetDirection();
	enum directions{FRONT, LEFT, RIGHT};
	int SetCurrentMode(int mode);
    int accelerate(void);
    int getSpeed();
    void setSetPointYaw(float spy);
    float getSetPointYaw();

private:
    Devices* currDevices;
    uint32_t currcoderead;
    int direction;
    int seed;
    int random_index;
    int next_index;
    int currentMode;
    int nextMode;
    Timer planningTimer;
    int acceleration_0to100 = 2;
    int deceleration_100to0 = 1;
    int accelerationRate;
    int decelerationRate;
    int currentSpeed = 0;
    int maxlimitModeSpeed;
    int minlimitModeSpeed;
    float setPointYaw;
};
#endif

