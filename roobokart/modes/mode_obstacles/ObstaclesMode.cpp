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

#include "../../../roobokart/modes/mode_obstacles/ObstaclesMode.h"


// Constructor
ObstaclesMode::ObstaclesMode(Serial* ser,Devices* devices,int yourmode, Planning *planning)
{
	mymode = yourmode;
	currDevices = devices;
}


int ObstaclesMode::runMode(void)
{
#ifdef DEBUG_OBSTACLES_MODE
	printf("Nav Mode: It works!\r\n");
#endif

	currentmode = mymode;
	currDevices->tof->display(currentmode);

	while(currentmode == mymode){



	}
	return currentmode;
}
