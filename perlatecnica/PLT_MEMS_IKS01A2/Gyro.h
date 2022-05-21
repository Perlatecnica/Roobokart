/**
 ******************************************************************************
 * @file    Gyro.h
 * @author  Francesco Caiazzo
 * @version V2.0.0
 * @date    13-May-2022
 * @brief   Header file for class Gyro header of the gyro library,
 *          it uses Seb Madgwick's Fusion library to calculate the attitude 
 *          asynchronously using the LSM6DSL sensor
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2022 perlatecnica.it, MIT License</center></h2>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 */

#ifndef PLT_MEMS_GYRO_H
#define PLT_MEMS_GYRO_H

#include "mbed.h"
#include "Fusion/Fusion.h"
#include "LSM6DSLSensor.h" 

class Gyro
{    
    LSM6DSLSensor acc_gyro;
    Thread attitude_thread;
    
    const uint32_t sampleRate = 100;

    const FusionMatrix gyroscopeMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    const FusionVector gyroscopeSensitivity = {-0.001f, -0.001f, 0.001f};
    const FusionVector gyroscopeOffset = {0.0f, 0.0f, 0.0f};

    const FusionMatrix accelerometerMisalignment = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    const FusionVector accelerometerSensitivity = {0.001f, 0.001f, -0.001f};
    const FusionVector accelerometerOffset = {0.0f, 0.0f, 0.0f};

    FusionOffset offset;
    FusionAhrs ahrs;    

    const FusionAhrsSettings settings = { .gain = 0.5f, .accelerationRejection = 10.0f,
        .magneticRejection = 20.0f, .rejectionTimeout = 5 * sampleRate, /* 5 seconds */ };    
    
    FusionEuler euler;
    float offset_roll = 180.0f;
    float offset_pitch = 0.0f;
    float offset_yaw = 0.0f;

    public:
    Gyro() = delete;
    Gyro(DevI2C * dev_i2c);
    Gyro(const Gyro &) = delete;
    Gyro(Gyro &&) = delete;  

    Gyro & operator=(const Gyro &) = delete;
    Gyro & operator=(Gyro &&) = delete;  
    
    inline float getRoll() { return range(euler.angle.roll - offset_roll); }
    inline float getPitch() { return range(euler.angle.pitch - offset_pitch); }
    inline float getYaw() { return range(euler.angle.yaw - offset_yaw); }

    inline void resetRoll() { offset_roll = 180.0f; }
    inline void resetPitch() { offset_pitch = 0.0f; }
    inline void resetYaw() { offset_yaw = 0.0f; }
 
    inline const float & getRollOffset() { return offset_roll; }
    inline const float & getPitchOffset() { return offset_pitch; }
    inline const float & getYawOffset() { return offset_yaw; }

    inline void setRollOffset() { offset_roll = euler.angle.roll; }
    inline void setPitchOffset() { offset_pitch = euler.angle.pitch; }
    inline void setYawOffset() { offset_yaw = euler.angle.yaw; }

    inline void setRollOffset(const float degrees) { offset_roll = degrees; }
    inline void setPitchOffset(const float degrees) { offset_pitch = degrees; }
    inline void setYawOffset(const float degrees) { offset_yaw = degrees; }

    void start();
    void stop();
    bool waitForYawStab(const uint32_t timeout_ms = 20000, const uint8_t max_count = 100, const float threshold = 0.01f);
    
    private:
    static float range(float value);
    void _attitude_thread();
};

#endif