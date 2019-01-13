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

#include "../../../roobokart/modes/mode_nav_stop/NavModeStop.h"

// Constructor
NavModeStop::NavModeStop(Serial* ser,Devices* devices,int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
	currPlanning = planning;
}


int NavModeStop::runMode(void)
{
#ifdef DEBUG_NAV_MODE_STOP
	printf("Nav Mode: It works!\r\n");
#endif

	currentmode = mymode;
	currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	while(currentmode == mymode){

		// Reading IRs value
		rfrontIR = currDevices->rfrontIR->read();
		lfrontIR = currDevices->lfrontIR->read();
		cfrontIR = currDevices->cfrontIR->read();

		targetdirection = lfrontIR - rfrontIR;

		if (targetdirection > 0)
		{
			direction = (int8_t) ((targetdirection - 0.3)*75);
		}
		else
		{
			direction = (int8_t) ((targetdirection - 0.5)*80);
		}


#ifdef DEBUG_NAV_MODE_STOP
		printf("rfrontIR: %f\r\n",rfrontIR);
		printf("lfrontIR: %f\r\n",lfrontIR);
		printf("cfrontIR: %f\r\n",cfrontIR);
		printf("direction: %3d\r\n",direction);
		printf("\033[4A");
#endif

		if(currDevices->roadsignDetected(cfrontIR)){
			currDevices->currMotors.run(0,-5, MOTOR_LEFT , MOTOR_RIGHT);
			// Stop Bot
			currentmode = NULL_MODE;
			break;
		}
		else
		{
			// The duckiebot navigates
			currDevices->currMotors.run(direction, 50, MOTOR_LEFT , MOTOR_RIGHT);
		}
	}
	return currentmode;
}
