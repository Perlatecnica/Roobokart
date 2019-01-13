/**
 ******************************************************************************
  * @file    STSpin240_250_def.h 
  * @author  IPC Rennes
  * @version V1.0.0
  * @date    February 10, 2016
  * @brief   Header for STSpin240 (low voltage dual brush DC motor driver) and 
  *          STSpin250 driver (low voltage  brush DC motor driver)
  * @note    (C) COPYRIGHT 2016 STMicroelectronics
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STSPIN240_250_H
#define __STSPIN240_250_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "STSpin240_250_config.h"
#include "motor_def.h"   
   
/* Definitions ---------------------------------------------------------------*/

/** @addtogroup Components
  * @{
  */   
   
/** @defgroup STSPIN240_250 STSPIN240_250
  * @{
  */   
   
/** @defgroup STSPIN240_250_Exported_Defines STSPIN240_250 Exported Defines
  * @{
  */   

/// Current FW version
   
/// Current FW major version
#define STSPIN240_250_FW_MAJOR_VERSION (uint8_t)(1)
/// Current FW minor version
#define STSPIN240_250_FW_MINOR_VERSION (uint8_t)(0)
/// Current FW patch version
#define STSPIN240_250_FW_PATCH_VERSION (uint8_t)(0)
/// Current FW version
#define STSPIN240_250_FW_VERSION (uint32_t)((STSPIN240_250_FW_MAJOR_VERSION<<16)|\
                                            (STSPIN240_250_FW_MINOR_VERSION<<8)|\
                                            (STSPIN240_250_FW_PATCH_VERSION))   

///Max number of Brush DC motors
#define STSPIN240_250_NB_MAX_MOTORS (2)
///Number of Bridges
#define STSPIN240_250_NB_BRIDGES (2)
    
    /**
  * @}
  */
     
/* Types ---------------------------------------------------------------------*/

/** @defgroup STSPIN240_250_Exported_Types STSPIN240_250 Exported Types
  * @{
  */   


/** @defgroup Device_Parameters Device Parameters
  * @{
  */

/// Device Parameters Structure Type
typedef struct {
    /// Pwm frequency of the bridge input
    uint32_t bridgePwmFreq[STSPIN240_250_NB_BRIDGES];      
    /// Pwm frequency of the ref pin
    uint32_t refPwmFreq;      
    /// Pwm Duty Cycle of the ref pin
    uint8_t refPwmDc;      
    /// Speed% (from 0 to 100) of the corresponding motor
     uint16_t speed[STSPIN240_250_NB_MAX_MOTORS];  
    /// FORWARD or BACKWARD direction of the motors
    motorDir_t direction[STSPIN240_250_NB_MAX_MOTORS];                 
    /// Current State of the motors
     motorState_t motionState[STSPIN240_250_NB_MAX_MOTORS];       
    /// Current State of the bridges
    bool bridgeEnabled[STSPIN240_250_NB_BRIDGES];    
    /// Enabling of a dual bridge configuration
    bool dualBridgeEnabled;    
}deviceParams_t; 

/**
  * @}
  */



/** @defgroup STSpin240_250_Initialization_Structure STSpin240_250 Initialization Structure
  * @{
  */
/* ACTION --------------------------------------------------------------------*
 * Declare here the component's initialization structure, if any, one         *
 * variable per line without initialization.                                  *
 *                                                                            *
 * Example:                                                                   *
 *   typedef struct                                                           *
 *   {                                                                        *
 *     int frequency;                                                         *
 *     int update_mode;                                                       *
 *   } COMPONENT_init_t;                                                      *
 *----------------------------------------------------------------------------*/  
typedef struct {
  /* Frequency of the bridge PWMs in Hz up to 100000Hz */
  uint32_t bridgePwmFreq[STSPIN240_250_NB_BRIDGES];
  /* Frequency of the PWM used for Ref pin in Hz up to 100000Hz */
  uint32_t refPwmFreq; 
  /* Duty cycle of the PWM used for Ref pin in % (0..100) */
  uint8_t refPwmDc;
  /* Dual Bridge configuration  (0 for mono, 1 for dual brush dc (STSPIN240 only) */
  bool dualBridgeEnabled;
} STSpin240_250_init_t;

/**
  * @}
  */

/** 
 * @brief  Powerstep01 driver data structure definition.
 */ 
/* ACTION --------------------------------------------------------------------*
 * Declare here the structure of component's data, if any, one variable per   *
 * line without initialization.                                               *
 *                                                                            *
 * Example:                                                                   *
 *   typedef struct                                                           *
 *   {                                                                        *
 *       int T0_out;                                                          *
 *       int T1_out;                                                          *
 *       float T0_degC;                                                       *
 *       float T1_degC;                                                       *
 *   } COMPONENT_Data_t;                                                      *
 *----------------------------------------------------------------------------*/

/**
  * @}
  */

/**
  * @}
  */

/* Functions -----------------------------------------------------------------*/


/** @defgroup STSPIN240_250_Board_Linked_Functions STSPIN240_250 Board Linked Functions
  * @{
  */  

/* ACTION --------------------------------------------------------------------*
 * Declare here extern platform-dependent APIs you might need (e.g.: I/O and  *
 * interrupt related functions), and implement them in a glue-logic file on   *
 * the target environment, for example within the "x_nucleo_board.c" file.    *
 * E.g.:                                                                      *
 *   extern status_t COMPONENT_IO_Init (void *handle);                        *
 *   extern status_t COMPONENT_IO_Read (handle, buf, regadd, bytes);          *
 *   extern status_t COMPONENT_IO_Write(handle, buf, regadd, bytes);          *
 *   extern void     COMPONENT_IO_ITConfig(void);                             *
 *----------------------------------------------------------------------------*/
    
///Delay of the requested number of milliseconds
extern void STSpin240_250_Board_Delay(void *handle, uint32_t delay);         
///Disable the bridges
extern void STSpin240_250_Board_DisableBridge(void *handle);     
///Enable the specified bridge
extern void STSpin240_250_Board_EnableBridge(void *handle, uint8_t addDelay);      
//Get the status of the flag and enable Pin
extern uint8_t STSpin240_250_Board_GetFaultPinState(void *handle);
//Get the status of the reset Pin
extern uint8_t STSpin240_250_Board_GetResetPinState(void *handle);
///Set Briges Inputs PWM frequency and start it
extern void STSpin240_250_Board_PwmSetFreq(void *handle, uint8_t pwmId, uint32_t newFreq, uint8_t duty); 
///Deinitialise the PWM of the specified bridge input
extern void STSpin240_250_Board_PwmDeInit(void *handle, uint8_t pwmId);
///init the PWM of the specified bridge input
extern void STSpin240_250_Board_PwmInit(void *handle, uint8_t pwmId, uint8_t onlyChannel);    
///Stop the PWM of the specified brigde input
extern void STSpin240_250_Board_PwmStop(void *handle, uint8_t pwmId);
///Release the reset pin of the STSpin240 or STSpin250
extern void STSpin240_250_Board_ReleaseReset(void *handle, uint8_t deviceId); 
///Reset the STSpin240 or STSpin250
extern void STSpin240_250_Board_Reset(void *handle, uint8_t deviceId);  
///Set direction of the specified bridge
extern void STSpin240_250_Board_SetDirectionGpio(void *handle, uint8_t bridgeId, uint8_t gpioState); 


/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
  }
#endif

#endif /* #ifndef __STSPIN240_250_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
