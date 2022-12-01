/*
 * roobokart_v3_pins.h
 *
 *  Created on: 26 ago 2022
 *      Author: Francesco
 */

#ifndef ROOBOKART_ROOBOKART_V3_PINS_H_
#define ROOBOKART_ROOBOKART_V3_PINS_H_

// ST7735 Display
#define DISPLAY_CS  PD_2
#define DISPLAY_A0  PC_1
#define DISPLAY_LED PC_0

//Front IR sensors
#define CENTRE_FRONT_IR  PC_3
#define LEFT_FRONT_IR    PC_2
#define RIGHT_FRONT_0_IR PB_1
#define RIGHT_FRONT_1_IR PC_4
#define RIGHT_FRONT_2_IR PC_5

/* XNucleoIKS01A2 MEMS */
#define IKS01A2_INT1     PA_6
#define IKS01A2_INT2     PA_15

/* XNucleo IHM12A1 */
#define F_E_PIN         PC_14
#define STANDBY_RST_PIN PC_15
#define DIR_A_PIN       PH_0
#define DIR_B_PIN       PH_1
#define PWM_A_PIN       PC_8
#define PWM_B_PIN       PC_6
#define PWM_REF_PIN     PC_9

/* BUZZER */
#define BUZZER_PIN      PA_11

/* HC05 */
#define BT_TX PA_9
#define BT_RX PA_10

/* 74HC595 */
#define SHIFT_REG_LE PB_12

#endif /* ROOBOKART_ROOBOKART_V3_PINS_H_ */
