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

#include "../../../roobokart/modes/mode_nav/NavMode.h"


// Constructor
NavMode::NavMode(Serial* ser,Devices* devices,int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
	//dirPID = new PID(100, -100, 60, 1, 50); //prima del cambio motore
	dirPID = new PID(100, -100, NAV_KP, NAV_KD, NAV_KI); // Setting 1
	//dirPID = new PID(100, -100, 50, 20, 25); // Setting 2
	//dirPID = new PID(100, -100, 85, 4, 6);

	currPlanning = planning;
}


int NavMode::runMode(void)
{
#ifdef DEBUG_NAV_MODE
	printf("Nav Mode: It works!\r\n");
#endif

	float speed = 0;// = currPlanning->getSpeed() ;
	int roadsigndetected = 0; // It stores the information that the road sign has been detected
	float det = 0; // deltaT for PID evaluation

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	// SetPoinDirection evaluated at calibration time
	spd = currDevices->getSPDirection(); //0.3

	pidtimer.start();
	dirPID->reset();

	while(currentmode == mymode){

		// Reading IRs value
		rfrontIR = currDevices->readRightIR();
		lfrontIR = currDevices->readLeftIR();
		cfrontIR = currDevices->readCentreIR();

		//speed = currPlanning->accelerate();

		// Evaluating PID for direction correction
		det = pidtimer.read();
		currentdirection = dirPID->evaluate(det, spd, rfrontIR);
		pidtimer.reset();


		// Roobokart navigates
		if(roadsigndetected == 0)
		{
			if(currDevices->roadsignDetected(cfrontIR))
			{
				// Once the road sign has been detected, the Roobokart proceeds at slower speed
				currDevices->currMotors.turn(0, MIN_NAV_SPEED, MOTOR_LEFT, MOTOR_RIGHT);
				roadsigndetected = 1;
			}
			else
			{
				// The Roobokart navigates until the road sign is detected
				speed = Devices::map(1.0f-abs(spd-rfrontIR), 0.0f, 1.0f, MIN_NAV_SPEED, CRUISE_NAV_SPEED);
				currDevices->currMotors.car(-currentdirection, speed, MOTOR_LEFT, MOTOR_RIGHT);
			}
		}
		// Road sign detected - managing the handshake with next mode
		else
		{
			// The Roobokart is stopped as soon as the cfrontIR sensor is over the blue line
			if(!currDevices->roadsignDetected(cfrontIR))
			{
				currDevices->currMotors.stop();
				// Roadsign detected, it switches to next mode
				currentmode = nextmode;
				roadsigndetected = 0;
				break;
			}
		}

#ifdef DEBUG_NAV_MODE
		printf("rfrontIR: %f\r\n",rfrontIR);
		printf("lfrontIR: %f\r\n",lfrontIR);
		printf("cfrontIR: %f\r\n",cfrontIR);
		printf("direction: %4.2f\r\n",currentdirection);
		printf("speed: %f\r\n",speed);
		printf("roadsigndetected: %d\r\n",roadsigndetected);
		printf("\033[6A");
#endif

		wait_ms(10);
	} // end while

	// Restore initial condition
	pidtimer.stop();
	pidtimer.reset();
	return currentmode;
}
