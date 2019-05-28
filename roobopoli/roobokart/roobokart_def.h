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

#ifndef DUCKIEBOT_DEF_H
#define DUCKIEBOT_DEF_H

//--INCLUDE--//
#include "../roobokart/modes/modes.h"
#include "mbed.h"
#include "debug.h"

#define DIR_FRONT	0
#define DIR_LEFT	1
#define DIR_RIGHT	2

#define IR_THRESHOLD 0.8
#define ROADSIGN_DETECTION_THRESHOLD_DEFAULT 0.8

#define RED_THRESHOLD_DEFAULT 160
#define GREEN_THRESHOLD_DEFAULT 310

#define BRAKING_FORCE_DEFAULT -12

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
#define F_E_PIN D2
#define STANDBY_RST_PIN D9
#define DIR_A_PIN   D7//D6
#define DIR_B_PIN   D6//D7
#define PWM_A_PIN   D4//D5
#define PWM_B_PIN   D5//D4
#define PWM_REF_PIN A0

#endif
