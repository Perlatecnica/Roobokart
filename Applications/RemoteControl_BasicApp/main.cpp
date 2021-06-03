/* Copyright (c) 2021-2022 Perlatecnica no-profit organization
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

#include "mbed.h"

#ifdef ROOBOKART_REMOTE_CONTROL_BASICAPP

#include "mbed.h"
#include "Devices.h"

//Serial connection to PC
Serial pc(USBTX, USBRX);
Devices devices(&pc);

int main() {
	char bt_received;
	pc.baud(9600);

	pc.printf("\r\n\r\n");
	pc.printf("\r\n\tREMOTE CONTROL BASIC APP v1.0.0\r\n");



	int speed = 100;
	while(true) {
		//pc.printf("Reading\r\n");
		while (devices.BT_hc05->readable()){
				bt_received = devices.BT_hc05->getc();
				pc.printf("Dato Ricevuto: %c\r\n",bt_received);

				switch(bt_received){

				case 'L': // Left
					devices.currMotors.speed(MOTOR_RIGHT,speed); //1
					devices.currMotors.speed(MOTOR_LEFT,0); // 0
					break;

				case 'F': // Forward
					devices.currMotors.speed(MOTOR_RIGHT,speed); //1
					devices.currMotors.speed(MOTOR_LEFT,speed); // 0
					break;

				case 'R': // Rigth
					devices.currMotors.speed(MOTOR_RIGHT,0); //1
					devices.currMotors.speed(MOTOR_LEFT,speed); // 0
					break;

				case 'B': // Back
					devices.currMotors.speed(MOTOR_RIGHT,-speed); //1
					devices.currMotors.speed(MOTOR_LEFT,-speed); // 0
					break;


				case ' ': // STOP
					devices.currMotors.speed(MOTOR_RIGHT,0); //1
					devices.currMotors.speed(MOTOR_LEFT,0); // 0
					break;

				default: // STOP
					devices.currMotors.speed(MOTOR_RIGHT,0); //1
					devices.currMotors.speed(MOTOR_LEFT,0); // 0
					break;
				}

		}
	}
}
#endif
