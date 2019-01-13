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
	//dirPID = new PID(100, -100, 70, 0, 0); // breadbord bianca
	dirPID = new PID(100, -100, 95, 10, 20); // breadbord bianca

	currPlanning = planning;
}


int NavMode::runMode(void)
{
#ifdef DEBUG_NAV_MODE
	printf("Nav Mode: It works!\r\n");
#endif

	int speed = currPlanning->getSpeed() ;
	int roadsigndetected = 0; // It stores the information that
	double det = 0; // deltaT for PID evaluation

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	// SetPoinDirection evaluated at calibration time
	spd = currDevices->getSPDirection(); //0.3

	pidtimer.start();

	navstatus = NORMAL_NAV_STATUS;

	while(currentmode == mymode){

		// Reading IRs value
		rfrontIR = currDevices->rfrontIR->read();
		lfrontIR = currDevices->lfrontIR->read();
		cfrontIR = currDevices->cfrontIR->read();

		// The Duckiebot shall follow a constant value of the difference of the values of IR Sensors, left and right.
		// The constant value is evaluated at Calibration time
		currentdirection = lfrontIR - rfrontIR;

		// Managing the speed
		// Check if the duckiebot leaved the white lane on the right side
		if(currentdirection==0) {
			// The Duckiebot leaved the whit lane on the righ side, decelerate in order to help the PID algo
			// The bot decelerate till the min cruise vel
			// Speed is linearly decreased
			if(speed > MIN_NAV_SPEED) {
				speed =  speed - 2;
			}
			else {
				navstatus = HARD_CURVE_STATUS;
			}

		}else {
			// Accelerate till max cruise vel
			speed = currPlanning->accelerate(); //50;
			navstatus = NORMAL_NAV_STATUS;
		}

		// Evaluating PID for direction correction
		det = (double)pidtimer.read();
		direction = (int8_t)(dirPID->evaluate(det,spd,currentdirection)); //0.3
		pidtimer.reset();


#ifdef DEBUG_NAV_MODE
		printf("rfrontIR: %f\r\n",rfrontIR);
		printf("lfrontIR: %f\r\n",lfrontIR);
		printf("cfrontIR: %f\r\n",cfrontIR);
		printf("direction: %3d\r\n",direction);
		printf("\033[4A");
#endif


		switch(navstatus){

		// Managing normal navigation
		// In this status the roadsign can be detected
		case NORMAL_NAV_STATUS:
			// Duckiebot navigates
			if(roadsigndetected == 0) {
				// Check if the road sign is detected
				if(currDevices->roadsignDetected(cfrontIR)){
					// Once the road sign has been detected, the duckiebot proceeds at slower speed
					currDevices->currMotors.turn(0, 40, MOTOR_LEFT , MOTOR_RIGHT);
					roadsigndetected = 1;
				}else {
					// The duckiebot navigates until the road sign is detected
					currDevices->currMotors.turn(direction, speed, MOTOR_LEFT , MOTOR_RIGHT);
				}
			}
			// Road sign detected - managing the handshake with next mode
			else {
				// The Duckiebot is stopped as soon as the cfrontIR sensor is over the blue line
				if(!currDevices->roadsignDetected(cfrontIR)){
					currDevices->currMotors.stop(); //.turn(0, 0, MOTOR_LEFT , MOTOR_RIGHT);

					// Roadsign detected, it switches to next mode
					currentmode = nextmode;
					break;
				}
			}
			break;

		// Managing Hard curve on the right
		// This status manages only the curve. It isn't possible to detect the roadsign
		// No controls, only lane search based on PID algo
		case HARD_CURVE_STATUS:
			currDevices->currMotors.turn(direction, speed, MOTOR_LEFT , MOTOR_RIGHT);
			break;


		} // end switch
		wait_ms(10);//40
	} // end while

	// Restore initial condition
	pidtimer.stop();
	pidtimer.reset();
	return currentmode;
}


