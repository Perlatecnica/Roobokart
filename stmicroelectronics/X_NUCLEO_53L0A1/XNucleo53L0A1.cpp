/**
 ******************************************************************************
 * @file    XNucleo53L0A1.cpp
 * @author  IMG
 * @version V1.0.0
 * @date    28-November-2016
 * @brief   Implementation file for the X_NUCLEO_VL53L0A1 singleton class
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
#include "XNucleo53L0A1.h"
#include "VL53L0X_platform_log.h"

/* Static variables ----------------------------------------------------------*/
XNucleo53L0A1 *XNucleo53L0A1::_instance = NULL;

XNucleo53L0A1 *XNucleo53L0A1::instance(DevI2C *ext_i2c)
{
    if (_instance == NULL)
        _instance = new XNucleo53L0A1(ext_i2c);
    else
        VL53L0X_ErrLog("Failed to create XNucleo53L0A1 instance\n\r");
    return _instance;
}

XNucleo53L0A1 *XNucleo53L0A1::instance(DevI2C *ext_i2c,
        PinName gpio1_centre,
        PinName gpio1_left, PinName gpio1_right)
{
    if (_instance == NULL)
        _instance = new XNucleo53L0A1(ext_i2c, gpio1_centre, gpio1_left, gpio1_right);
    else
        VL53L0X_ErrLog("Failed to create XNucleo53L0A1 instance\n\r");
    return _instance;
}


int XNucleo53L0A1::init_board()
{
    int status, n_dev = 0;

								
  
					   
 
    sensor_centre->VL53L0X_off();
    sensor_left->VL53L0X_off();
    sensor_right->VL53L0X_off();
    status = sensor_centre->init_sensor(NEW_SENSOR_CENTRE_ADDRESS);
    if (status) {
	
        delete sensor_centre;
        delete xshutdown_centre;
        sensor_centre = NULL;
        xshutdown_centre = NULL;
        printf("Sensor centre not present\n\r");
	
    } else {
	
        printf("Sensor centre present\n\r");
        n_dev++;
    }
    status = sensor_left->init_sensor(NEW_SENSOR_LEFT_ADDRESS);
    if (status) {
	
        delete sensor_left;
        delete xshutdown_left;
        sensor_left = NULL;
        xshutdown_left = NULL;
        printf("Sensor left not present\n\r");
	
    } else {
	
        printf("Sensor left present\n\r");
        n_dev++;
    }

    status = sensor_right->init_sensor(NEW_SENSOR_RIGHT_ADDRESS);
    if (status) {
	
        delete sensor_right;
        delete xshutdown_right;
        sensor_right = NULL;
        xshutdown_right = NULL;
        printf("Sensor right not present\n\r");
	
    } else {
	
        printf("Sensor right present\n\r");
        n_dev++;
    }

    if (n_dev == 0) {
        return 1;
    } else {
        return 0;
    }
}
