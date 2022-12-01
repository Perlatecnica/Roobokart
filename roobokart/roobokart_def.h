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


//#define ROOBOKART_MASTER_V1 // Roobokart old version board
#define ROOBOKART_MASTER_V3 // Roobokart new version board
//#define ROOBOKART_A // Roobokart A old board

/* SPECIFIC ROOBOKART DEFINITIONS */
/* THE FOLLOWING PARAMETERS CAN BE MODIFIED */
/* EACH ROOBOKART COULD HAVE DIFFERENT VALUES */
#ifdef ROOBOKART_MASTER_V1
#define IR_THRESHOLD 0.5
#define ROADSIGN_DETECTION_THRESHOLD_DEFAULT 0.5
#define COLOR_THRESHOLD 0.3f
#define BRAKING_FORCE_DEFAULT -10
#define NAV_KP 48
#define NAV_KI 8
#define NAV_KD 0
#define RSA_KP 1.2f				// road sign align gyro kp
#define LINE_FOLLOWER_SP 0.50f	// line follower set point
#define    MAX_NAV_SPEED 100	// navigation mode max speed
#define    MIN_NAV_SPEED 28		// navigation mode min speed
#define CRUISE_NAV_SPEED 35		// navigation mode cruise speed
#define CRUISE_RSA_SPEED 28		// road sign align mode cruise speed
#define    SEEK_TL_SPEED 30		// traffic light seek speed
#define  ESCAPE_TL_SPEED 40		// traffic light escape speed

#define   IR_L_MIN_VALUE 0.039805f
#define   IR_L_MAX_VALUE 1.000000f
#define   IR_C_MIN_VALUE 0.047375f
#define   IR_C_MAX_VALUE 1.000000f
#define   IR_R_MIN_VALUE 0.052991f
#define   IR_R_MAX_VALUE 1.000000f

#endif

#ifdef ROOBOKART_MASTER_V3
#define IR_THRESHOLD 0.5
#define ROADSIGN_DETECTION_THRESHOLD_DEFAULT 0.5
#define COLOR_THRESHOLD 0.3f
#define BRAKING_FORCE_DEFAULT -15
#define NAV_KP 52
#define NAV_KI 8
#define NAV_KD 0
#define RSA_KP 1.2f				// road sign align gyro kp
#define LINE_FOLLOWER_SP 0.40f	// line follower set point
#define    MAX_NAV_SPEED 100	// navigation mode max speed
#define    MIN_NAV_SPEED 30 	// navigation mode min speed
#define CRUISE_NAV_SPEED 35	 	// navigation mode cruise speed
#define CRUISE_RSA_SPEED 30		// road sign align mode cruise speed
#define    SEEK_TL_SPEED 35		// traffic light seek speed
#define  ESCAPE_TL_SPEED 45		// traffic light escape speed

#define   IR_L_MIN_VALUE 0.054212f
#define   IR_L_MAX_VALUE 0.852259f
#define   IR_C_MIN_VALUE 0.054701f
#define   IR_C_MAX_VALUE 0.808303f
#define   IR_R_MIN_VALUE 0.047863f
#define   IR_R_MAX_VALUE 0.730647f

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
#define CRUISE_NAV_SPEED 30 	// navigation mode cruise speed
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



//DEBUG
#define IR_DEBUG

//Motors
#define MOTOR_LEFT		0
#define MOTOR_RIGHT		1

/* BUSES */
#undef  I2C_SDA
#define I2C_SDA D14

#undef  I2C_SCL
#define I2C_SCL D15

#undef  I2C1_SDA
#define I2C1_SDA I2C_SDA

#undef  I2C1_SCL
#define I2C1_SCL I2C_SCL

#undef  I2C2_SDA
#define I2C2_SDA PB_3

#undef  I2C2_SCL
#define I2C2_SCL PB_10

#undef  I2C3_SDA
#define I2C3_SDA PB_4

#undef  I2C3_SCL
#define I2C3_SCL PA_8

#undef SPI3_MOSI
#define SPI3_MOSI PC_12

#undef SPI3_MISO
#define SPI3_MISO PC_11

#undef SPI3_SCK
#define SPI3_SCK  PC_10

#ifdef ROOBOKART_V3
#include "roobokart_v3_pins.h"
#else
#include "roobokart_v1_pins.h"
#endif

#endif
