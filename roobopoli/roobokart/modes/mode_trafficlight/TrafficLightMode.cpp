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

#include "../../../roobokart/modes/mode_trafficlight/TrafficLightMode.h"


// Constructor
TrafficLightMode::TrafficLightMode(Serial* ser,Devices* devices, int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
	currPlanning = planning;
}


int TrafficLightMode::runMode(void)
{
#ifdef DEBUG_TRAFFICLIGHT_MODE
	printf("TrafficLight Mode: It works!\r\n");
#endif

	int red = 0;
	int green = 0;

	int redthreshold = currDevices->getRedThreshold();
	int greenthreshold = currDevices->getGreenThreshold();

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	while(currentmode == mymode){
		//cfrontIR = currDevices->cfrontIR->read();
		red = currDevices->color->ReadRed();
		green = currDevices->color->ReadGreen();

		if(red < redthreshold & red < green){//200
			currDevices->currMotors.run(0, BRAKING_FORCE_DEFAULT, MOTOR_LEFT , MOTOR_RIGHT);
		} else if(green < greenthreshold & red > green){
			currentmode = nextmode;
			currDevices->color->SetMode(TCS3200::POWERDOWN);

			//vado un po avanti per superare il semaforo
			currDevices->currMotors.run(0, 35, MOTOR_LEFT , MOTOR_RIGHT);
			wait_ms(500);
			break;
		} //else {
//			currDevices->currMotors.run(0, 35, MOTOR_LEFT , MOTOR_RIGHT);//25
//		}
	}

	return currentmode;
}
