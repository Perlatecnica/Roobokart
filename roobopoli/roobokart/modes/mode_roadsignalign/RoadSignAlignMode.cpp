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

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);

	while(currentmode == mymode){
		rfrontIR = 0;
		lfrontIR = 0;
		cfrontIR = 0;

		for(int k=0;k<5;k++){
			rfrontIR += currDevices->rfrontIR->read();
			lfrontIR += currDevices->lfrontIR->read();
			cfrontIR += currDevices->cfrontIR->read();
			wait_ms(10);
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


		switch(status){
			case NOT_ALIGNED_STATUS:
				if(LEFT_IR_WHITE){
					currDevices->currMotors.speed(MOTOR_LEFT,40);
				} else {
					//currDevices->currMotors.stop(MOTOR_LEFT);
					currDevices->currMotors.speed(MOTOR_LEFT,BRAKING_FORCE_DEFAULT);
				}

				if(RIGHT_IR_WHITE){
					currDevices->currMotors.speed(MOTOR_RIGHT,40);
				} else {
					//currDevices->currMotors.stop(MOTOR_RIGHT);
					currDevices->currMotors.speed(MOTOR_RIGHT,BRAKING_FORCE_DEFAULT);
				}

				if((RIGHT_IR_BLACK) & (LEFT_IR_BLACK) ){
					currDevices->mems->calibrateLSM6DSL(50);
					// It stores the current yaw as setpoint
					currDevices->mems->compute();
					float setPointYaw = currDevices->mems->attitude.yaw;
					currPlanning->setSetPointYaw(setPointYaw);
					wait_ms(500);
					currDevices->currMotors.run(0, 40, MOTOR_LEFT , MOTOR_RIGHT);
					wait_ms(100);
					currDevices->currMotors.run(0, 30, MOTOR_LEFT , MOTOR_RIGHT);
					status = READ_CODE_STATUS;
				}
				break;



			case READ_CODE_STATUS:
				if (righIR_previousvalue && RIGHT_IR_BLACK){

					// Writing the read bit value at lsb position
					if(LEFT_IR_BLACK) roadsignvalue = roadsignvalue | mask1;
					else roadsignvalue = roadsignvalue = roadsignvalue & mask0;

					//currDevices->tof->display(message);
					//currDevices->tof->display(roadsignvalue);

					bitcount++;
					righIR_previousvalue = false;
					if(bitcount > BITS_TO_BE_READ){
						currDevices->tof->display((int)roadsignvalue);
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
				//currDevices->currMotors.stop();
				// Switch ON the sensor of color
				currDevices->color->SetMode(TCS3200::SCALE_20);
				// It informs the planning about the read value of cross road. Planning is in charge of choosing the next direction
				currPlanning->setCrossCodeRead(roadsignvalue);
				break;

		}

	} // end while
	return currentmode;
}
