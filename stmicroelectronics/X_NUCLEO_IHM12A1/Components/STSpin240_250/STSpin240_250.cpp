/**
 ******************************************************************************
 * @file    STSpin240_250.cpp
 * @author  IPC Rennes
 * @version V1.0.0
 * @date    April 25th, 2016
 * @brief   STSpin240 motor driver (Dual brush DC motors)
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

/* Includes ------------------------------------------------------------------*/
#include "STSpin240_250.h"

/* Definitions ---------------------------------------------------------------*/

/* Error: Access a motor index greater than the one of the current brigde configuration */
#define STSPIN240_250_ERROR_1   (0xC001)
/* Error: Use of a bridgeId greater than BRIDGE_B */
#define STSPIN240_250_ERROR_2   (0xC002)    

/* Maximum frequency of the PWMs in Hz */
#define STSPIN240_250_MAX_PWM_FREQ   (100000)

/* Minimum frequency of the PWMs in Hz */
#define STSPIN240_250_MIN_PWM_FREQ   (2)

/* Bridge A */
#define BRIDGE_A         (0)

/* Bridge B */
#define BRIDGE_B         (1)
    
/* PWM id for PWM_A */
#define PWM_A         (0)

/* PWM id for PWM_B */
#define PWM_B         (1)

/* PWM id for PWM_REF */
#define PWM_REF       (2)    

/* Variables  ----------------------------------------------------------------*/

/* Number of devices. */
uint8_t STSpin240_250::numberOfDevices = 0;
uint8_t STSpin240_250::arrayNbMaxMotorsByConfig[2] = {1,2};
/* Methods -------------------------------------------------------------------*/

/**********************************************************
 * @brief Starts the STSpin240_250 library
 * @param[in] pInit pointer to the initialization data
 * @retval COMPONENT_OK in case of success.
 **********************************************************/
status_t STSpin240_250::STSpin240_250_Init(void* pInit)
{ 
    /* Standby-reset deactivation */
    STSpin240_250_Board_ReleaseReset();
    
    /* Let a delay after reset */
    STSpin240_250_Board_Delay(1);
    
    if (pInit == 0)
    {
        // Set all registers to their predefined values 
        // from powerstep01_target_config.h 
        STSpin240_250_SetDeviceParamsToPredefinedValues();
    }
    else
    {
        STSpin240_250_SetDeviceParamsToGivenValues((STSpin240_250_init_t *)pInit);
    }
    
    /* Initialise input PWM of bridges*/
    STSpin240_250_SetDualFullBridgeconfig(device_prm.dualBridgeEnabled);
    
    return COMPONENT_OK;
}

/**********************************************************
 * @brief Read id
 * @param[in] id pointer to the identifier to be read.
 * @retval COMPONENT_OK in case of success.
 **********************************************************/
status_t STSpin240_250::STSpin240_250_ReadId(uint8_t *id)
{
    *id = deviceInstance;
    
    return COMPONENT_OK;
}

/**********************************************************
 * @brief  Attaches a user callback to the error Handler.
 * The call back will be then called each time the library 
 * detects an error
 * @param[in] callback Name of the callback to attach 
 * to the error Hanlder
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_AttachErrorHandler(void (*callback)(uint16_t error))
{
    errorHandlerCallback = (void (*)(uint16_t error)) callback;
}


/******************************************************//**
 * @brief Disable the specified bridge
 * @param[in] bridgeId (from 0 for bridge A to 1 for bridge B)
 * @retval None
 * @note  Bridge A and bridge B share the same enable pin. 
 * When bridge A is disabled, bridge B is disabled and 
 * reversely
 **********************************************************/
void STSpin240_250::STSpin240_250_DisableBridge(uint8_t bridgeId)
{
    STSpin240_250_Board_DisableBridge();
    device_prm.bridgeEnabled[BRIDGE_A] = FALSE;
    device_prm.bridgeEnabled[BRIDGE_B] = FALSE;
}

/******************************************************//**
 * @brief Enable the specified bridge
 * @param[in] bridgeId (from 0 for bridge A to 1 for bridge B)
 * @retval None
 * @note  Bridge A and bridge B share the same enable pin. 
 * When bridge A is enabled, bridge B is enabled and 
 * reversely
 **********************************************************/
void STSpin240_250::STSpin240_250_EnableBridge(uint8_t bridgeId)
{
    device_prm.bridgeEnabled[BRIDGE_A] = TRUE;
    device_prm.bridgeEnabled[BRIDGE_B] = TRUE;  
    STSpin240_250_Board_EnableBridge(1);
}

/**********************************************************
 * @brief Error handler which calls the user callback (if defined)
 * @param[in] error Number of the error
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_ErrorHandler(uint16_t error)
{
    if (errorHandlerCallback != 0)
    {
    (void) errorHandlerCallback(error);
    }
    else   
    {
    /* Aborting the program. */
    exit(EXIT_FAILURE);
    }
}

/******************************************************//**
 * @brief  Get the PWM frequency of the specified bridge 
 * @param[in] bridgeId 0 for bridge A, 1 for bridge B
 * @retval Freq in Hz
 **********************************************************/
uint32_t STSpin240_250::STSpin240_250_GetBridgeInputPwmFreq(uint8_t bridgeId)
{                                                  
  if (bridgeId > BRIDGE_B)
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_2);
  }    
  
  return (device_prm.bridgePwmFreq[(bridgeId << 1)]);
}

/******************************************************//**
 * @brief  Get the status of the bridge enabling of the corresponding bridge
 * @param[in] bridgeId from 0 for bridge A to 1 for bridge B
 * @retval State of the Enable&Fault pin (shared for bridge A and B)
  **********************************************************/
uint16_t STSpin240_250::STSpin240_250_GetBridgeStatus(void)
{
    uint16_t status = STSpin240_250_Board_GetFaultPinState();
  
    return (status);
}

/******************************************************//**
 * @brief  Returns the current speed of the specified motor
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @retval current speed in % from 0 to 100
 **********************************************************/
uint16_t STSpin240_250::STSpin240_250_GetCurrentSpeed(uint8_t motorId)
{
  uint16_t speed = 0;

  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  else
  {
      if (device_prm.motionState[motorId] != INACTIVE)
      {
          speed = device_prm.speed[motorId];
      }
  }
  return (speed); 
}

/******************************************************//**
 * @brief Returns the device state
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @retval State (STEADY or INACTIVE)
 **********************************************************/
motorState_t STSpin240_250::STSpin240_250_GetDeviceState(uint8_t motorId)
{
  motorState_t state =  INACTIVE;

  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  else
  {
    state =  device_prm.motionState[motorId];
  }
  return (state);      
}

/******************************************************//**
 * @brief Get the motor current direction
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @retval direction
 **********************************************************/
motorDir_t STSpin240_250::STSpin240_250_GetDirection(uint8_t motorId)
{
  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  
  return device_prm.direction[motorId];
}

/******************************************************//**
 * @brief Returns the FW version of the library
 * @retval STSPIN240_250_FW_VERSION
 **********************************************************/
uint32_t STSpin240_250::STSpin240_250_GetFwVersion(void)
{
  return (STSPIN240_250_FW_VERSION);
}


/******************************************************//**
 * @brief  Return the duty cycle of PWM used for REF 
 * @param[in] refId 0 is the only supported id for STSpin240 or 
 * STSpin250
 * @retval duty cycle in % (from 0 to 100)
 **********************************************************/
uint8_t STSpin240_250::STSpin240_250_GetRefPwmDc(uint8_t refId)
{                                                  
  uint32_t duty = 0;
  
  if (duty == 0)
  {
    duty = device_prm.refPwmDc;
  }
  return (duty);
}

/******************************************************//**
 * @brief  Return the frequency of PWM used for REF 
 * @param[in] refId 0 is the only supported id for STSpin240 or 
 * STSpin250
 * @retval Frequency in Hz
 **********************************************************/
uint32_t STSpin240_250::STSpin240_250_GetRefPwmFreq(uint8_t refId)
{                                                  
  uint32_t freq = 0;
  
  if (refId == 0)
  {
    freq = device_prm.refPwmFreq;
  }
  return (freq);
}

/******************************************************//**
 * @brief  Immediatly stops the motor and disable the power bridge
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @retval None
 * @note  As all motors uses the same power bridge, the 
 * power bridge will be disable only if all motors are
 * stopped
 **********************************************************/
void STSpin240_250::STSpin240_250_HardHiz(uint8_t motorId) 
{
   if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  else
  {
    if (device_prm.bridgeEnabled[motorId] != FALSE)
    {
      /* Only disable bridges if dual bridge mode is disabled or */
      /* if all motors are inactive as there are sharing the same power stage */
      if ((device_prm.dualBridgeEnabled == 0)||
          ((motorId == 0)&&(device_prm.motionState[1] == INACTIVE))||
          ((motorId == 1)&&(device_prm.motionState[0] == INACTIVE)))
      {  
        /* Disable the bridge */
        STSpin240_250_DisableBridge(motorId);
      }
    }
    /* Disable the PWM */
    STSpin240_250_HardStop(motorId);
  }
}

/******************************************************//**
 * @brief  Stops the motor without disabling the bridge
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @retval none
 **********************************************************/
void STSpin240_250::STSpin240_250_HardStop(uint8_t motorId)
{   
  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  else
  {
      if (device_prm.motionState[motorId] != INACTIVE)
      {
        /* Disable corresponding PWM */
        STSpin240_250_Board_PwmStop(motorId);
    
        /* Set inactive state */
        device_prm.motionState[motorId] = INACTIVE;
      }
  }
}

/******************************************************//**
 * @brief Release reset (exit standby mode)
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES -1)
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_ReleaseReset(void)
{
  STSpin240_250_Board_ReleaseReset();
  
  /* Start PWM used for REF pin */
  STSpin240_250_Board_PwmSetFreq(PWM_REF, device_prm.refPwmFreq,device_prm.refPwmDc);          
}

/******************************************************//**
 * @brief Reset (enter standby mode)
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES -1)
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_Reset(void)
{
  uint8_t loop;
  for (loop = 0; loop < STSPIN240_250_NB_MAX_MOTORS; loop++)
  {
    /* Stop motor if needed*/
    if (device_prm.motionState[loop] != INACTIVE) 
    {
      STSpin240_250_HardStop(loop);
    }
    /* Disable bridge if needed */
    if (device_prm.bridgeEnabled[loop] != FALSE) 
    {
      STSpin240_250_DisableBridge(loop);
    }    
  }
  
  /* Stop PWM used for REF pin */
  STSpin240_250_Board_PwmStop(PWM_REF);
  
  /* Reset the STBY/RESET pin */
  STSpin240_250_Board_Reset();
}

/******************************************************//**
 * @brief  Runs the motor
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @param[in] direction FORWARD or BACKWARD
 * @retval None
 * @note  For unidirectionnal motor, direction parameter has 
 * no effect
 **********************************************************/
void STSpin240_250::STSpin240_250_Run(uint8_t motorId, motorDir_t direction)
{
  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }  
  else
  {
      if ((device_prm.motionState[motorId] == INACTIVE) ||
          (device_prm.direction[motorId] != direction))
      {
        
        /* Release reset if required */
        if (STSpin240_250_GetResetState() == 0)
        {
          STSpin240_250_ReleaseReset();
        }
        
        /* Eventually deactivate motor */
        if (device_prm.motionState[motorId] != INACTIVE) 
        {
          STSpin240_250_HardStop(motorId);
        }
    
        /* Set direction */
        STSpin240_250_SetDirection(motorId, direction);
    
        /* Switch to steady state */
        device_prm.motionState[motorId] = STEADY;
       
        /* Enable bridge */
        if (device_prm.bridgeEnabled[motorId] == FALSE)
        {
          STSpin240_250_EnableBridge(motorId);
        }
        /* Set PWM */
        STSpin240_250_Board_PwmSetFreq(motorId, device_prm.bridgePwmFreq[motorId],device_prm.speed[motorId]);          
      }
  }
}

/******************************************************//**
 * @brief  Changes the PWM frequency of the bridge input
 * @param[in] bridgeId 0 for bridge A, 1 for bridge B
 * @param[in] newFreq in Hz
 * @retval None
 * @note 1)The PWM is only enabled when the motor is requested
 * to run.
 * 2) If the two bridges share the same timer, their frequency
 * has to be the same
 * 3) If the two bridges share the same timer, the frequency
 * is updated on the fly is there is only one motor running
 * on the targeted bridge.
 **********************************************************/
void STSpin240_250::STSpin240_250_SetBridgeInputPwmFreq(uint8_t bridgeId, uint32_t newFreq)
{                                                  
  if (bridgeId > BRIDGE_B)
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_2);
  }  
  
  if (newFreq > STSPIN240_250_MAX_PWM_FREQ)
  {
    newFreq = STSPIN240_250_MAX_PWM_FREQ;
  }
 
  device_prm.bridgePwmFreq[bridgeId] = newFreq;
  
  if (device_prm.motionState[bridgeId] != INACTIVE)
  {
    STSpin240_250_Board_PwmSetFreq(bridgeId, device_prm.bridgePwmFreq[bridgeId],device_prm.speed[bridgeId]);          
  }
}

/******************************************************//**
 * @brief  Specifies the direction 
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @param[in] dir FORWARD or BACKWARD
 * @note The direction change is only applied if the device 
 * is in INACTIVE state. To change direction while motor is 
 * running, use the run function
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_SetDirection(uint8_t motorId, motorDir_t dir)
{
   if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }  
  else
  {
      if (device_prm.motionState[motorId] == INACTIVE)
      {
        STSpin240_250_Board_SetDirectionGpio(motorId, dir);
        device_prm.direction[motorId] = dir;
      }
  }
}

/******************************************************//**
 * @brief Set the dual bridge configuration mode
 * @param[in] enable 0 to disable, 
 *                   1 to enable (not supported by STSPIN250)
 * @retval None
  **********************************************************/
void STSpin240_250::STSpin240_250_SetDualFullBridgeconfig(uint8_t enable)
{
  device_prm.dualBridgeEnabled = enable;

  /* Check reset pin state*/
  if (STSpin240_250_GetResetState() != 0)
  {
    STSpin240_250_Reset();
    STSpin240_250_ReleaseReset();
  }
}

/******************************************************//**
 * @brief  Changes the max speed of the specified device
 * @param[in] motorId from 0 to (MAX_NUMBER_OF_BRUSH_DC_MOTORS - 1) 
 * @param[in] newMaxSpeed in % from 0 to 100
 * @retval true if the command is successfully executed, else false
 **********************************************************/
bool STSpin240_250::STSpin240_250_SetMaxSpeed(uint8_t motorId, uint16_t newMaxSpeed)
{                                                  
  bool cmdExecuted = FALSE;

  if (motorId >= arrayNbMaxMotorsByConfig[device_prm.dualBridgeEnabled])
  {
    STSpin240_250_ErrorHandler(STSPIN240_250_ERROR_1);
  }
  else
  {
    device_prm.speed[motorId] = newMaxSpeed;
    if (device_prm.motionState[motorId] != INACTIVE)
    {
      /* Set PWM frequency*/
      STSpin240_250_Board_PwmSetFreq(motorId, device_prm.bridgePwmFreq[motorId],device_prm.speed[motorId]);          
    }
    cmdExecuted = TRUE;
  }
  return cmdExecuted;
}      

/******************************************************//**
 * @brief  Changes the duty cycle of the PWM used for REF 
 * @param[in] refId 0 is the only supported id for STSpin240 or 
 * STSpin250
 * @param[in] newDc new duty cycle from 0 to 100
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_SetRefPwmDc(uint8_t refId, uint8_t newDc)
{                                                  
  if (newDc > 100)
  {
    newDc = 100;
  }
 
  device_prm.refPwmDc = newDc;
  
  if (STSpin240_250_GetResetState() != 0)
  {
    /* Immediatly set the PWM frequency  for ref if chip is not in reset */
    STSpin240_250_Board_PwmSetFreq(PWM_REF, device_prm.refPwmFreq,device_prm.refPwmDc);          
  }
}

/******************************************************//**
 * @brief  Changes the frequency of PWM used for REF 
 * @param[in] refId 0 is the only supported id for STSpin240 or 
 * STSpin250
 * @param[in] newFreq in Hz
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_SetRefPwmFreq(uint8_t refId, uint32_t newFreq)
{
  if (newFreq > STSPIN240_250_MAX_PWM_FREQ)
  {
    newFreq = STSPIN240_250_MAX_PWM_FREQ;
  }
 
  device_prm.refPwmFreq = newFreq;
  
  if (STSpin240_250_GetResetState() != 0)
  {
    /* Immediatly set the PWM frequency  for ref if chip is not in reset */
    STSpin240_250_Board_PwmSetFreq(PWM_REF, device_prm.refPwmFreq,device_prm.refPwmDc);            
  }
} 


/******************************************************//**
 * @brief  Get the status of the bridge enabling of the corresponding bridge
 * @retval State of the Enable&Fault pin (shared for bridge A and B)
  **********************************************************/
uint8_t STSpin240_250::STSpin240_250_GetResetState(void)
{
  uint8_t status = STSpin240_250_Board_GetResetPinState();
  
  return (status);
}

/******************************************************//**
 * @brief  Set the parameters of the device to values of pInitPrm structure
 * @param[in] deviceId (from 0 to MAX_NUMBER_OF_DEVICES -1)
 * @param pInitPrm pointer to a structure containing the initial device parameters 
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_SetDeviceParamsToGivenValues(STSpin240_250_init_t *pInitPrm)
{
  uint32_t i;

  device_prm.dualBridgeEnabled = pInitPrm->dualBridgeEnabled;

  device_prm.bridgePwmFreq[BRIDGE_A] = pInitPrm->bridgePwmFreq[BRIDGE_A];
  device_prm.bridgePwmFreq[BRIDGE_B] = pInitPrm->bridgePwmFreq[BRIDGE_B];;

  device_prm.refPwmFreq = pInitPrm->refPwmFreq;
  device_prm.refPwmDc = pInitPrm->refPwmDc;
  
  for (i = 0; i < MAX_NUMBER_OF_BRUSH_DC_MOTORS; i++)
  {
    device_prm.speed[i] = 100;
    device_prm.direction[i] = FORWARD;
    device_prm.motionState[i] = INACTIVE;
  }
  for (i = 0; i < STSPIN240_250_NB_BRIDGES; i++)
  {  
    device_prm.bridgeEnabled[i] = FALSE;
  }  
}
/******************************************************//**
 * @brief  Sets the parameters of the device to predefined values
 * from STSpin240_250_target_config.h
 * @retval None
 **********************************************************/
void STSpin240_250::STSpin240_250_SetDeviceParamsToPredefinedValues(void)
{
  uint32_t i;

  device_prm.dualBridgeEnabled = STSPIN240_250_CONF_PARAM_DUAL_BRIDGE_ENABLING;

  device_prm.bridgePwmFreq[BRIDGE_A] = STSPIN240_250_CONF_PARAM_FREQ_PWM_A;
  device_prm.bridgePwmFreq[BRIDGE_B] = STSPIN240_250_CONF_PARAM_FREQ_PWM_B;

  device_prm.refPwmFreq = STSPIN240_250_CONF_PARAM_FREQ_PWM_REF;
  device_prm.refPwmDc = STSPIN240_250_CONF_PARAM_DC_PWM_REF;
  
  for (i = 0; i < MAX_NUMBER_OF_BRUSH_DC_MOTORS; i++)
  {
    device_prm.speed[i] = 100;
    device_prm.direction[i] = FORWARD;
    device_prm.motionState[i] = INACTIVE;
  }
  for (i = 0; i < STSPIN240_250_NB_BRIDGES; i++)
  {  
    device_prm.bridgeEnabled[i] = FALSE;
  }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
