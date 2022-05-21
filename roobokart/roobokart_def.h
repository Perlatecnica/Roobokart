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

#ifndef ROOBOKART_DEF_H
#define ROOBOKART_DEF_H

//--INCLUDE--//
#include "../roobokart/modes/modes.h"
#include "mbed.h"
#include "debug.h"


#define ROOBOKART_MASTER // Roobokart Rosso Master Demo new board
//#define ROOBOKART_A // Roobokart A old board

/* SPECIFIC ROOBOKART DEFINITIONS */
/* THE FOLLOWING PARAMETERS CAN BE MODIFIED */
/* EACH ROOBOKART COULD HAVE DIFFERENT VALUES */
#ifdef ROOBOKART_MASTER
#define IR_THRESHOLD 0.8
#define ROADSIGN_DETECTION_THRESHOLD_DEFAULT 0.5
#define COLOR_THRESHOLD 0.3f
#define BRAKING_FORCE_DEFAULT -10
#define NAV_KP 64
#define NAV_KI 50
#define NAV_KD 1
#define RSA_KP 1.2f				// road sign align gyro kp
#define    MAX_NAV_SPEED 100	// navigation mode max speed
#define    MIN_NAV_SPEED 20		// navigation mode min speed
#define CRUISE_NAV_SPEED 45		// navigation mode cruise speed
#define CRUISE_RSA_SPEED 40		// road sign align mode cruise speed
#define    SEEK_TL_SPEED 35		// traffic light seek speed
#define  ESCAPE_TL_SPEED 40		// traffic light escape speed
#endif


#ifdef ROOBOKART_A
#define IR_THRESHOLD 0.8
#define ROADSIGN_DETECTION_THRESHOLD_DEFAULT 0.2 /**/
#define COLOR_THRESHOLD 0.3f
#define BRAKING_FORCE_DEFAULT -10
#define NAV_KP 64
#define NAV_KI 50
#define NAV_KD 1
#define RSA_KP 1.2f				// road sign align gyro kp
#define MAX_NAV_SPEED 100		// navigation mode max speed
#define MIN_NAV_SPEED 20		// navigation mode min speed
#define CRUISE_NAV_SPEED 35 	// navigation mode cruise speed
#define    SEEK_TL_SPEED 30		// traffic light seek speed
#define  ESCAPE_TL_SPEED 35		// traffic light escape speed
#endif


/* DON'T MODIFY THE FOLLOWING */
/* GENERAL ROOBOKART DEFINITIONS */

#define DIR_FRONT	0
#define DIR_LEFT	1
#define DIR_RIGHT	2

#define RIGHT_IR_WHITE  rfrontIR <= IR_THRESHOLD
#define LEFT_IR_WHITE  lfrontIR <= IR_THRESHOLD

#define RIGHT_IR_BLACK  rfrontIR > IR_THRESHOLD
#define LEFT_IR_BLACK  lfrontIR > IR_THRESHOLD

//A0 is used by the motor shield
//A2 and A3 are busy

//DEBUG
#define IR_DEBUG

//Front IR sensors
#define RIGHT_FRONT_IR 		A5
#define CENTRE_FRONT_IR 	A1
#define LEFT_FRONT_IR		A4

//Motors
#define MOTOR_LEFT		0
#define MOTOR_RIGHT		1

/* BUSES */
#define I2C_SDA D14
#define I2C_SCL D15

/* XNucleoIKS01A2 MEMS */
#define IKS01A2_I2C_SDA D14
#define IKS01A2_I2C_SCL D15
#define IKS01A2_INT1    D11
#define IKS01A2_INT2    D12


/* Color Sensor */
#define TCS3200_S0  PB_14
#define TCS3200_S1  PB_13
#define TCS3200_S2  PB_1
#define TCS3200_S3  PB_15
#define TCS3200_OUT PB_2

/* XNucleo IHM12A1 */
#define F_E_PIN PB_3//D2
#define STANDBY_RST_PIN PB_6//D9
#define DIR_A_PIN   D7//D6
#define DIR_B_PIN   D6//D7
#define PWM_A_PIN   D4//D5
#define PWM_B_PIN   D5//D4
#define PWM_REF_PIN A0
#define BUZZER_PIN PC_9

/* HC05 */
#define BT_TX PA_9
#define BT_RX PA_10
#endif
