/**
 ******************************************************************************
 * @file    Gyro.cpp
 * @author  Francesco Caiazzo
 * @version V2.0.0
 * @date    13-May-2022
 * @brief   Implementation file for the Gyro class
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

#include "Gyro.h"

Gyro::Gyro(DevI2C * dev_i2c) : acc_gyro(dev_i2c)
{
    acc_gyro.init(NULL);
    acc_gyro.enable_x();
    acc_gyro.enable_g();
    FusionOffsetInitialise(&offset, sampleRate);
    FusionAhrsInitialise(&ahrs);
    FusionAhrsSetSettings(&ahrs, &settings);
}

void Gyro::start()
{        
    attitude_thread.start(callback(this,&Gyro::_attitude_thread));
}

void Gyro::stop()
{        
    attitude_thread.terminate();
}

bool Gyro::waitForYawStab(const uint32_t timeout_ms, const uint8_t max_count, const float threshold)
{
    uint8_t count = 0;
    float old_val = getYaw();
    float cur_val = -old_val;
    Timer timeout;
    timeout.start();
    do
    {
        ThisThread::sleep_for(20);
        cur_val = getYaw();
        if (abs(cur_val - old_val) < threshold) 
        {
            count++;
        }
        else
        {
            if (count>0) count--;
            old_val = cur_val;
        }
    }
    while ((count <= max_count) && (timeout.read_ms() < timeout_ms));
    timeout.stop();
    return count > 10;
}

float Gyro::range(float value)
{
    if (value<=-180) return value + 360;
    if (value>180) return value - 360;
    return value;
}

void Gyro::_attitude_thread()
{
    int32_t axes_acc[3];
    int32_t axes_gyr[3];
    FusionVector gyroscope;
    FusionVector accelerometer;
    clock_t timestamp;
    clock_t previousTimestamp = clock();
    float deltaTime;

    while (true)
    {
        timestamp = clock(); // replace this with actual gyroscope timestamp
        
        acc_gyro.get_x_axes(axes_acc);
        acc_gyro.get_g_axes(axes_gyr);        
                   
        gyroscope.axis.x = axes_gyr[0];
        gyroscope.axis.y = axes_gyr[1];
        gyroscope.axis.z = axes_gyr[2];
            
        accelerometer.axis.x = axes_acc[0];
        accelerometer.axis.y = axes_acc[1];
        accelerometer.axis.z = axes_acc[2];
              
        gyroscope = FusionCalibrationInertial(gyroscope, gyroscopeMisalignment, gyroscopeSensitivity, gyroscopeOffset);
        accelerometer = FusionCalibrationInertial(accelerometer, accelerometerMisalignment, accelerometerSensitivity, accelerometerOffset);
    
        gyroscope = FusionOffsetUpdate(&offset, gyroscope);
                    
        deltaTime = (float) (timestamp - previousTimestamp) / (float) CLOCKS_PER_SEC;
        previousTimestamp = timestamp;
        
        FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, deltaTime);

        euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));
    }
}