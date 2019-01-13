/**
 ******************************************************************************
 * @file    XNucleoIKS01A2.h
 * @author  CLab
 * @version V1.0.0
 * @date    9-August-2016
 * @brief   Header file for class XNucleoIKS01A2 representing a X-NUCLEO-IKS01A2
 *          expansion board
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

/* Define to prevent from recursive inclusion --------------------------------*/
#ifndef __X_NUCLEO_IKS01A2_H
#define __X_NUCLEO_IKS01A2_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "x_nucleo_iks01a2_targets.h"
#include "HTS221Sensor.h"
#include "LSM303AGRAccSensor.h"
#include "LSM303AGRMagSensor.h"
#include "LPS22HBSensor.h"
#include "LSM6DSLSensor.h"
#include "DevI2C.h"

/* Macros -------------------------------------------------------------------*/
#define CALL_METH(obj, meth, param, ret) ((obj == NULL) ?       \
                      ((*(param) = (ret)), 0) : \
                      ((obj)->meth(param))      \
                      )

/* Classes -------------------------------------------------------------------*/
/** Class XNucleoIKS01A2 is intended to represent the MEMS Inertial & Environmental 
 *  Nucleo Expansion Board with the same name.
 *
 *  The expansion board is featuring basically four IPs:\n
 *  -# a HTS221 Relative Humidity and Temperature Sensor\n
 *  -# a LSM303AGR 3-Axis Magnetometer and 3D Acceleromenter\n
 *  -# a LPS22HB MEMS Pressure Sensor (and Temperature Sensor)\n
 *  -# and a LSM6DSL 3D Acceleromenter and 3D Gyroscope\n
 *
 * The expansion board features also a DIL 24-pin socket which makes it possible
 * to add further MEMS adapters and other sensors (e.g. UV index). 
 *
 * It is intentionally implemented as a singleton because only one
 * X_NUCLEO_IKS01A2 at a time might be deployed in a HW component stack.\n
 * In order to get the singleton instance you have to call class method `Instance()`, 
 * e.g.:
 * @code
 * // Inertial & Environmental expansion board singleton instance
 * static X_NUCLEO_IKS01A2 *<TODO>_expansion_board = X_NUCLEO_IKS01A2::Instance();
 * @endcode
 */
class XNucleoIKS01A2
{
 protected:
    XNucleoIKS01A2(DevI2C *ext_i2c, PinName int1, PinName int2);

    ~XNucleoIKS01A2(void) {
        /* should never be called */
        error("Trial to delete XNucleoIKS01A2 singleton!\n");
    }

 public:
    static XNucleoIKS01A2* instance(DevI2C *ext_i2c = NULL, PinName int1 = IKS01A2_PIN_LSM6DSL_INT1, PinName int2 = IKS01A2_PIN_LSM6DSL_INT2);
    static XNucleoIKS01A2* instance(PinName sda, PinName scl, PinName int1 = IKS01A2_PIN_LSM6DSL_INT1, PinName int2 = IKS01A2_PIN_LSM6DSL_INT2);

    DevI2C  *dev_i2c;

    HTS221Sensor  *ht_sensor;
    LSM303AGRMagSensor *magnetometer;
    LSM303AGRAccSensor *accelerometer;
    LPS22HBSensor  *pt_sensor;
    LSM6DSLSensor *acc_gyro;

 private:
    static XNucleoIKS01A2 *_instance;
};

#endif /* __X_NUCLEO_IKS01A2_H */
