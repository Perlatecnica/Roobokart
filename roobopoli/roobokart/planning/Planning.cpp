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

#include "../../roobokart/planning/Planning.h"

#define DIR_FORWARD 0
#define DIR_LEFT 1
#define DIR_RIGHT 2

	// The input value crosscoderead is [0,3]
	// This function will choose randomly
	// 00 FRONT, LEFT, 	RIGHT
	// 01 FRONT, 		RIGHT
	// 10 FRONT, LEFT
	// 11 		 LEFT, 	RIGHT

int cross[4][3] = {{1,1,1},
				   {1,0,1},
				   {1,1,0},
				   {0,1,1}};

// Constructor
Planning::Planning(Devices* devices)
{
	currDevices = devices;
	seed = 78;
	planningTimer.start();
	accelerationRate = acceleration_0to100*10; //(acceleration_0to100*1000)/100;
	decelerationRate = deceleration_100to0*10;

}


/**
 * It informs the planning about tha value of the cross road read.
 */
void Planning::setCrossCodeRead(uint32_t crosscoderead){
	currcoderead = crosscoderead;

	// initialize random seed
	srand(seed);
	// next seed will be a random number
	seed = rand() % 1000 +1;

	// generate random number between 0 and 2. It is the index of the column in the cross matrix
	random_index = rand() % 3;
	// if the value selected in the cross matrix is not allowed (=0), the next column will be choosen
	next_index = random_index + 1;

	// control if next index exceed the max column number
	if(next_index > 2) next_index = 0;

	// select the direction
	if(cross[currcoderead][random_index] == 1)
		direction = directions(random_index);
	else direction = directions(next_index);
}

uint32_t Planning::getCrossCode(){
	return currcoderead;
}

/**
 * It return the last direction choosen by planning
 */
int Planning::GetDirection(){
	return direction;
}

/***
 * It informs the plannig about the current mode. Each mode is in charge of calling that method.
 * Planning return the next mode
 */
int Planning::SetCurrentMode(int mode){
	currentMode = mode;
	switch(currentMode){

	case RESET_MODE:
		nextMode = POST_CAL_MODE;// CROSS_MANAGEMENT_MODE;//
		break;

	case POST_CAL_MODE:
		nextMode = NAV_MODE;
		break;

	case NAV_MODE:
		nextMode = ROAD_SIGN_ALIGNING_MODE;
		maxlimitModeSpeed = CRUISE_NAV_SPEED;
		minlimitModeSpeed = MIN_NAV_SPEED;
		break;

	case ROAD_SIGN_ALIGNING_MODE:
		nextMode = TRAFFICLIGHT_MANAGEMENT_MODE;
		break;

	case TRAFFICLIGHT_MANAGEMENT_MODE:
		nextMode = NAV_MODE;
		//nextMode = CROSS_MANAGEMENT_MODE;
		break;

	case CROSS_MANAGEMENT_MODE:
		nextMode = NAV_MODE;
		break;

	/*case OBSTACLES_MANAGEMENT_MODE:
		currentmode = obstmode.runMode();
		break;
	case NAV_MODE_STOP:
		currentmode = navmodestop.runMode();
		break;*/

	default:
		nextMode = NULL_MODE;
		break;
	}
	return nextMode;
}

int Planning::accelerate(void){
	static int oldtime = 0;
	if(currentSpeed < maxlimitModeSpeed){
		if( (planningTimer.read_ms()-oldtime) > accelerationRate){
			currentSpeed++;
			oldtime = planningTimer.read_ms();
		}
	}
	return currentSpeed;
}

int Planning::getSpeed(){
	return currentSpeed;
}

void Planning::setSetPointYaw(float spy){
	setPointYaw = spy;
}

float Planning::getSetPointYaw(){
	return setPointYaw;
}
