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

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	currDevices->trafficLightReader->start();

	while(currentmode == mymode)
	{

#ifdef DEBUG_TRAFFICLIGHT_MODE

		while(true)
		{
			currDevices->trafficLightReader->read()
			printf("  RED: %f\r\n", currDevices->trafficLightReader->getRed());
			printf("GREEN: %f\r\n", currDevices->trafficLightReader->getGreen());
			printf("\r\n");
			wait(1);
		}
#endif



		switch (currDevices->trafficLightReader->read())
		{
		  case TrafficLightsReader::BLACK:
			currDevices->currMotors.run(0, SEEK_TL_SPEED, MOTOR_LEFT, MOTOR_RIGHT);
			break;

		  case TrafficLightsReader::GREEN:
			currentmode = nextmode;
			currDevices->trafficLightReader->stop();
			// robot goes a little further to pass the traffic light
			currDevices->currMotors.run(0, ESCAPE_TL_SPEED, MOTOR_LEFT, MOTOR_RIGHT);
			//wait_ms(500);
			printf("end -------------\r\n");
			wait_ms(1000);
			break;

		  default:
			  currDevices->currMotors.stop();
			  break;
		}
	}

	return currentmode;
}
