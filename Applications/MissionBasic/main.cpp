/* Copyright (c) 2021-2022 Perlatecnica no-profit organization
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

#include "mbed.h"

#ifdef ROOBOKART_BASIC_MISSION

#include "roobokart/devices/Devices.h"
#include "roobokart/modes/mode_cross/CrossMode.h"
#include "roobokart/modes/mode_nav/NavMode.h"
#include "roobokart/modes/mode_example/ExampleMode.h"
#include "roobokart/modes/mode_obstacles/ObstaclesMode.h"
#include "roobokart/modes/mode_post/PostMode.h"
#include "roobokart/modes/mode_roadsignalign/RoadSignAlignMode.h"
#include "roobokart/modes/mode_trafficlight/TrafficLightMode.h"
#include "roobokart/planning/Planning.h"
#include "roobokart/roobokart_def.h"


//Serial connection to PC
Serial pc(USBTX, USBRX);

int currentmode;

Devices devices(&pc);
Planning planning(&devices);
PostMode postmode(&pc,&devices, POST_CAL_MODE, &planning);
NavMode navmode(&pc,&devices, NAV_MODE, &planning);
RoadSignAlignMode rsamode(&pc,&devices, ROAD_SIGN_ALIGNING_MODE, &planning);
TrafficLightMode tlmode(&pc,&devices, TRAFFICLIGHT_MANAGEMENT_MODE, &planning);
CrossMode crossmode(&pc,&devices, CROSS_MANAGEMENT_MODE, &planning);
ObstaclesMode obstmode(&pc,&devices, OBSTACLES_MANAGEMENT_MODE, &planning);
//ExampleMode examplemode(&pc,&devices, NAV_MODE_STOP, &planning);



int main() {
	//It changes baudrate
	pc.baud(9600);
	pc.printf("\r\n\r\n");
	pc.printf("\r\n\tBASIC MISSION v1.0.0\r\n");

	currentmode = planning.SetCurrentMode(RESET_MODE);


	while(true) {

#ifdef DEBUG_PROXIMITY
		currentmode = -1; // It stops the state machine
		// The measurements of both the sensors will print on serial com
		devices.tof->getLeftMeasure();
		devices.tof->getRightMeasure();
		wait_ms(300);
#endif

		switch(currentmode){

		case POST_CAL_MODE:
			currentmode = postmode.runMode();
			break;

		case NAV_MODE:
			currentmode = navmode.runMode();
			//break is removed. It takes less time switching to the next mode that is the following ROAD_SIGN_ALIGNING_MODE

		case ROAD_SIGN_ALIGNING_MODE:
			currentmode = rsamode.runMode();
			break;

		case TRAFFICLIGHT_MANAGEMENT_MODE:
			currentmode = tlmode.runMode();
			break;

		case CROSS_MANAGEMENT_MODE:
			currentmode = crossmode.runMode();
			break;

		case OBSTACLES_MANAGEMENT_MODE:
			currentmode = obstmode.runMode();
			break;

		default:
			break;
		}
	}
}

#endif
