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

	bool ir_raw = false;
	do
	{
		printf("IR: %0.4f, %0.4f, %0.4f\r\n",
				currDevices->readLeftIR(ir_raw),
				currDevices->readCentreIR(ir_raw),
				currDevices->readRightIR(ir_raw));
	}while (false);

	/* Evaluating trafficlight colors */
	currDevices->trafficLightReader->start();

	do
	{
		currDevices->trafficLightReader->read();
		printf("Colors Threshold: %f\r\n", COLOR_THRESHOLD);
		printf("  RED: %f\r\n", currDevices->trafficLightReader->getRed());
		printf("GREEN: %f\r\n", currDevices->trafficLightReader->getGreen());
		printf(" BLUE: %f\r\n", currDevices->trafficLightReader->getBlue());
		wait_ms(1000);
	}
	while (false);

	currDevices->trafficLightReader->stop();

	wait_ms(300);
	/* Evaluating TOF */
	for (int i=0;i<10;i++)
	{
		currDevices->tof->getCentreMeasure();
		currDevices->tof->getLeftMeasure();
		currDevices->tof->getRightMeasure();
		wait_ms(100);
	}
	printf("Centre dist: %u\r\n", currDevices->tof->getCentreMeasure());
	printf("  Left dist: %u\r\n", currDevices->tof->getLeftMeasure());
	printf(" Right dist: %u\r\n", currDevices->tof->getRightMeasure());


	currDevices->tof->display("go");
	while( currDevices->usrButton->read() != 0 ){;}

	currDevices->buzz(7633,.5,1);
	while(currentmode == mymode){
		currentmode = nextmode;//NAV_MODE;
	}
	return currentmode;
}

void set_min(float & var, const float & value)
{
	if (var>value) var = value;
}

void set_max(float & var, const float & value)
{
	if (var<value) var = value;
}

int PostMode::calSPDirection(){

	int errcode = 0;
	float min;
	float max;

	float spd;

	min = currDevices->readRightIR(true); // rfrontIR->read();
	max = currDevices->readLeftIR(true);  // lfrontIR->read();

	for (int i = 0; i<10; i++ )
	{
		set_min(min, currDevices->readRightIR(true));
		set_max(max, currDevices->readLeftIR(true));
		wait_ms(10);
	}

	//spd = (min + max)/2 - ((min + max)*15/100); // spd is the average of the dx and sx reading - 30%
	spd = LINE_FOLLOWER_SP;

	currDevices->setMaxIR(max);
	printf("MaxIR: %f\r\n",max);
	currDevices->setMinIR(min);
	printf("MinIR: %f\r\n",min);
	currDevices->setSPDirection(spd);
	printf("SPDIR: %f\r\n",spd);
	return errcode;
}
