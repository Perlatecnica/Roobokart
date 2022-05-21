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

	/* Evaluating trafficlight colors */
	currDevices->trafficLightReader->start();
	currDevices->trafficLightReader->read();
	printf("Colors Threshold: %f\r\n", COLOR_THRESHOLD);
	printf("  RED: %f\r\n", currDevices->trafficLightReader->getRed());
	printf("GREEN: %f\r\n", currDevices->trafficLightReader->getGreen());
	printf(" BLUE: %f\r\n", currDevices->trafficLightReader->getBlue());

	wait_ms(300);
	currDevices->trafficLightReader->stop();

	currDevices->tof->display("go");
	while( currDevices->usrButton->read() != 0 ){;}

	currDevices->buzz(7633,.5,1);
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
