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

#include "../../../roobokart/modes/mode_roadsignalign/RoadSignAlignMode.h"

// Constructor
RoadSignAlignMode::RoadSignAlignMode(Serial* ser,Devices* devices,int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
	currPlanning = planning;

	dirPID_RS = new PID(100, -100, NAV_KP, NAV_KD, NAV_KI);
}


int RoadSignAlignMode::runMode(void)
{
#ifdef DEBUG_ROADSIGNALIGN_MODE
	printf("Roadsign aligning Mode: It works!\r\n");
#endif


	int status = NOT_ALIGNED_STATUS;
	int bitcount = 1;
	bool righIR_previousvalue = false;
	uint32_t roadsignvalue = 0;
	uint32_t mask0 = 0xFFFFFFFE;
	uint32_t mask1 = 0x00000001;
	double det = 0;

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);
	double speed;

	pidtimer_RS.start();
	dirPID_RS->reset();

	while(currentmode == mymode){
		rfrontIR = 0;
		lfrontIR = 0;
		cfrontIR = 0;

		for(int k=0;k<5;k++){
			rfrontIR += currDevices->rfrontIR->read();
			lfrontIR += currDevices->lfrontIR->read();
			cfrontIR += currDevices->cfrontIR->read();
			//wait_ms(10);
		}

		rfrontIR = rfrontIR/5;
		lfrontIR = lfrontIR/5;
		cfrontIR = cfrontIR/5;

#ifdef DEBUG_ROADSIGNALIGN_MODE
		printf("rfrontIR: %f\r\n",rfrontIR);
		printf("lfrontIR: %f\r\n",lfrontIR);
		printf("cfrontIR: %f\r\n",cfrontIR);
		printf("direction: %3d\r\n",direction);
		printf("\033[4A");
#endif
		printf("status %d\r\n", status);

		//speed = currPlanning->getSpeed();
		//speed = 0.8*speed;
		speed = CRUISE_RSA_SPEED;

		switch(status){
			case NOT_ALIGNED_STATUS:
				if(LEFT_IR_WHITE){
					currDevices->currMotors.speed(MOTOR_LEFT,speed);
				} else {
					currDevices->currMotors.speed(MOTOR_LEFT,BRAKING_FORCE_DEFAULT);
				}

				if(RIGHT_IR_WHITE){
					currDevices->currMotors.speed(MOTOR_RIGHT,speed);
				} else {
					currDevices->currMotors.speed(MOTOR_RIGHT,BRAKING_FORCE_DEFAULT);
				}

				if((RIGHT_IR_BLACK) && (LEFT_IR_BLACK) ){
					// It calibrates the MEMS sensors
					currDevices->mems->waitForYawStab(5000,100,0.1f);
					currDevices->mems->setYawOffset();
					setPointYaw = currDevices->mems->getYaw();
					currPlanning->setSetPointYaw(setPointYaw);
					wait_ms(300);
					status = READ_CODE_STATUS;
				}
				break;


			case READ_CODE_STATUS:
				yaw = currDevices->mems->getYaw();
				det = (double)pidtimer_RS.read();
				//direction_RS = (int8_t)(dirPID_RS->evaluate(det,setPointYaw,yaw));
				direction_RS = (int8_t)(currDevices->mems->getYaw()*RSA_KP);
				pidtimer_RS.reset();
				currDevices->currMotors.turn(-direction_RS, speed, MOTOR_LEFT , MOTOR_RIGHT);

				if (righIR_previousvalue && RIGHT_IR_BLACK){

					// Writing the read bit value at lsb position
					if(LEFT_IR_BLACK) roadsignvalue = roadsignvalue | mask1;
					else roadsignvalue = roadsignvalue = roadsignvalue & mask0;

					//currDevices->tof->display(message);
					//currDevices->tof->display(roadsignvalue);

					bitcount++;
					righIR_previousvalue = false;
					if(bitcount > BITS_TO_BE_READ){
						//currDevices->tof->display((int)roadsignvalue);
						status = PREPARE_NEXT_STATUS;
					}
					else {
						// New bit to be read. Shift the current value in order to free the lsb
						roadsignvalue = (roadsignvalue << 1);
						status = READ_CODE_STATUS;
					}
				}
				righIR_previousvalue = RIGHT_IR_WHITE;
				break;

			case PREPARE_NEXT_STATUS:
				currentmode = nextmode;

				// It informs the planning about the read value of cross road. Planning is in charge of choosing the next direction
				currPlanning->setCrossCodeRead(roadsignvalue);
				break;

		}

	} // end while
	pidtimer_RS.stop();
	pidtimer_RS.reset();
	return currentmode;
}
