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

#include "../../../roobokart/modes/mode_post/PostMode.h"

// Constructor
PostMode::PostMode(Serial* ser,Devices* devices,int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
	currPlanning = planning;
}


int PostMode::runMode(void)
{
#ifdef DEBUG_POST_MODE
	printf("Post Mode: It works!\r\n");
#endif

	currentmode = mymode;
	nextmode = currPlanning->SetCurrentMode(currentmode);
	currDevices->tof->display(currentmode);
	currDevices->tof->display("AUTO");
	wait_ms(50);
	currDevices->tof->display("CAL");
	wait_ms(500);
	currDevices->tof->display("MODE");
	wait_ms(500);

	// Evalutating Setpoint Direction
	currDevices->tof->display("spd");
	while( currDevices->usrButton->read() != 0 ){;}
	calSPDirection();

	/* Evaluating trafficlight colors thresholds*/
	currDevices->color->SetMode(TCS3200::SCALE_20);

	// Evaluating RED value
	wait_ms(500);
	int red = RED_THRESHOLD_DEFAULT;
	currDevices->setRedThreshold(red);
	printf("RED value: %d!\r\n",red);
	printf("RED Threshold: %d!\r\n",currDevices->getRedThreshold());

	// Evaluating GREEN value
	wait_ms(500);
	int green = GREEN_THRESHOLD_DEFAULT;
	currDevices->setGreenThreshold(green);
	printf("GREEN value: %d!\r\n",green);
	printf("GREEN Threshold: %d!\r\n",currDevices->getGreenThreshold());

	wait_ms(300);
	currDevices->color->SetMode(TCS3200::POWERDOWN);

	currDevices->tof->display("go");
	while( currDevices->usrButton->read() != 0 ){;}

	while(currentmode == mymode){
		currentmode = nextmode;//NAV_MODE;
	}
	return currentmode;
}


int PostMode::calSPDirection(){

	int errcode = 0;
	float rfrontIR;
	float lfrontIR;

	float spd;

	rfrontIR = currDevices->rfrontIR->read();
	lfrontIR = currDevices->lfrontIR->read();

	//spd = lfrontIR - rfrontIR;
	spd = (rfrontIR + lfrontIR)/2 - ((rfrontIR + lfrontIR)*15/100); // spd is the average of the dx and sx reading - 30%
	currDevices->setSPDirection(spd);
	wait(1);
	return errcode;
}
