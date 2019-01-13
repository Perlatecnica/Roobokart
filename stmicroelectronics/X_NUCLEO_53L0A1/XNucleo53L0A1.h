/**
 ******************************************************************************
 * @file    XNucleo53L0A1.h
 * @author  IMG
 * @version V1.0.0
 * @date    28-November-2016
 * @brief   Header file for class XNucleo53L0A1 representing a X-NUCLEO-53L0A1
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
#ifndef __XNucleo53L0A1_H
#define __XNucleo53L0A1_H

/* Includes ------------------------------------------------------------------*/
#include "VL53L0X.h"
#include "Display.h"
#include "Stmpe1600.h"
#include "DevI2C.h"

/** New device addresses */
#define NEW_SENSOR_CENTRE_ADDRESS		0x54
#define NEW_SENSOR_LEFT_ADDRESS			0x56
#define NEW_SENSOR_RIGHT_ADDRESS		0x58

/* Classes--------------------------------------------------------------------*/

/* Classes -------------------------------------------------------------------*/
/** Class representing the X-NUCLEO-VL53L0A1 expansion board
 */
class XNucleo53L0A1
{
public:
    /** Constructor 1
    * @param[in] &i2c device I2C to be used for communication
    */
    XNucleo53L0A1(DevI2C *ext_i2c) : dev_i2c(ext_i2c)
    {
        stmpe1600_exp0 = new Stmpe1600(*ext_i2c, (0x43 * 2));     // U21

        stmpe1600_exp1 = new Stmpe1600(*ext_i2c, (0x42 * 2));     // U19

        display = new Display(*stmpe1600_exp0, *stmpe1600_exp1);

        xshutdown_centre = new Stmpe1600DigiOut(*dev_i2c, GPIO_15, (0x42 * 2));     // U19 on schematic
        sensor_centre = new VL53L0X(*dev_i2c, *xshutdown_centre, A2);

        xshutdown_left = new Stmpe1600DigiOut(*dev_i2c, GPIO_14, (0x43 * 2));     // U21 on schematic
        sensor_left = new VL53L0X(*dev_i2c, *xshutdown_left, D8);

        xshutdown_right = new Stmpe1600DigiOut(*dev_i2c, GPIO_15, (0x43 * 2));     // U21 on schematic
        sensor_right = new VL53L0X(*dev_i2c, *xshutdown_right, D2);
    }

    /** Constructor 2
     * @param[in] &i2c device I2C to be used for communication
     * @param[in] PinName gpio1_top Mbed DigitalOut pin name to be used as a top sensor GPIO_1 INT
     * @param[in] PinName gpio1_bottom Mbed DigitalOut pin name to be used as a bottom sensor GPIO_1 INT
     * @param[in] PinName gpio1_left Mbed DigitalOut pin name to be used as a left sensor GPIO_1 INT
     * @param[in] PinName gpio1_right Mbed DigitalOut pin name to be used as a right sensor GPIO_1 INT
     */
    XNucleo53L0A1(DevI2C *ext_i2c, PinName gpio1_centre,
                    PinName gpio1_left, PinName gpio1_right) : dev_i2c(ext_i2c)
    {
        stmpe1600_exp0 = new Stmpe1600(*ext_i2c, (0x43 * 2));     // U21

        stmpe1600_exp1 = new Stmpe1600(*ext_i2c, (0x42 * 2));     // U19

        display = new Display(*stmpe1600_exp0, *stmpe1600_exp1);

        xshutdown_centre = new Stmpe1600DigiOut(*dev_i2c, GPIO_15, (0x42 * 2));     // U19 on schematic
        sensor_centre = new VL53L0X(*dev_i2c, *xshutdown_centre, gpio1_centre);

        xshutdown_left = new Stmpe1600DigiOut(*dev_i2c, GPIO_14, (0x43 * 2));     // U21 on schematic
        sensor_left = new VL53L0X(*dev_i2c, *xshutdown_left, gpio1_left);

        xshutdown_right = new Stmpe1600DigiOut(*dev_i2c, GPIO_15, (0x43 * 2));     // U21 on schematic
        sensor_right = new VL53L0X(*dev_i2c, *xshutdown_right, gpio1_right);
    }

    /** Destructor
     */
    ~XNucleo53L0A1()
    {
        if (xshutdown_centre != NULL) {
            delete xshutdown_centre;
            xshutdown_centre = NULL;
        }
        if (sensor_centre != NULL) {
            delete sensor_centre;
            sensor_centre = NULL;
        }
        if (xshutdown_left != NULL) {
            delete xshutdown_left;
            xshutdown_left = NULL;
        }
        if (sensor_left != NULL) {
            delete sensor_left;
            sensor_left = NULL;
        }
        if (xshutdown_right != NULL) {
            delete xshutdown_right;
            xshutdown_right = NULL;
        }
        if (sensor_right != NULL) {
            delete sensor_right;
            sensor_right = NULL;
        }

        delete stmpe1600_exp0;
        stmpe1600_exp0 = NULL;
        delete stmpe1600_exp1;
        stmpe1600_exp1 = NULL;
        delete display;
        display = NULL;
        _instance = NULL;
    }

    /**
     * @brief       Creates a singleton object instance
     * @param[in]   &i2c device I2C to be used for communication
     * @return      Pointer to the object instance
     */
    static XNucleo53L0A1 *instance(DevI2C *ext_i2c);

    /**
     * @brief       Creates a singleton object instance
     * @param[in]   &i2c device I2C to be used for communication
     * @param[in]   PinName gpio1_centre the pin connected to top sensor INT
     * @param[in]   PinName gpio1_left the pin connected to left sensor INT
     * @param[in]   PinName gpio1_right the pin connected to right sensor INT
     * @return      Pointer to the object instance
     */
    static XNucleo53L0A1 *instance(DevI2C *ext_i2c, PinName gpio1_centre,
                                     PinName gpio1_left, PinName gpio1_right);

    /**
     * @brief       Initialize the board and sensors with deft values
     * @return      0 on success
     */
    int init_board();

    DevI2C *dev_i2c;
    VL53L0X *sensor_centre;
    VL53L0X *sensor_left;
    VL53L0X *sensor_right;
    Stmpe1600 *stmpe1600_exp0;
    Stmpe1600 *stmpe1600_exp1;
    Stmpe1600DigiOut *xshutdown_centre;
    Stmpe1600DigiOut *xshutdown_left;
    Stmpe1600DigiOut *xshutdown_right;
    Display *display;

private:
    static XNucleo53L0A1 *_instance;
};

#endif /* __XNucleo53L0A1_H */


