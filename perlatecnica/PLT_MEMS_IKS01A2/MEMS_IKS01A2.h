/* Copyright (c) 2016-2017 perlatecnica.it, MIT License
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
*/

#ifndef MEMS_IKS01A2_H
#define MEMS_IKS01A2_H


#include "mbed.h"
#include "XNucleoIKS01A2.h"

#define MADGWICK_FILTER
//#define CREATE_AS_THREAD

#ifdef MAHONY_FILER
	#include <MahonyAHRS.h>
#endif
#ifdef MADGWICK_FILTER
	#include <MadgwickAHRS.h>
#endif

#define START_THREAD 1
#define DEFAULT_CAL_SAMPES 256

class MEMS_IKS01A2 {
  public:
	MEMS_IKS01A2();
	enum sensors {Magnetometer, Humidity, Pressure, Gyroscope, Accelerometer};
	LSM303AGRMagSensor *getMagnetometer();
	HTS221Sensor *getHumiditySensor();
	LPS22HBSensor *getPressureSensor();
	LSM6DSLSensor *getGyro();
	LSM303AGRAccSensor *getAccelerometer();

	void init(PinName sda, PinName scl, PinName int1, PinName int2);
	void calibrateLSM6DSL(int numsamples);
	void setODR(int ord);

	struct Attitude
	    {
	        float roll;
	        float pitch;
	        float yaw;
	    } attitude;

	//Attitude getAttitude(void);
	void compute();

  private:
	/* Instantiate the expansion board */
	 XNucleoIKS01A2 *mems_expansion_board;

	/* Retrieve the composing elements of the expansion board */
	 LSM303AGRMagSensor *magnetometer;// = mems_expansion_board->magnetometer;
	 HTS221Sensor *hum_temp;// = mems_expansion_board->ht_sensor;
	 LPS22HBSensor *press_temp;// = mems_expansion_board->pt_sensor;
	 LSM6DSLSensor *acc_gyro;// = mems_expansion_board->acc_gyro;
	 LSM303AGRAccSensor *accelerometer;// = mems_expansion_board->accelerometer;

	int32_t axesGyro[3];
	int32_t axesAcc[3];
	double g_xBias,g_yBias,g_zBias,a_xBias,a_yBias,a_zBias;

	Thread _thread;
	static void threadStarter(void const *p);
	void computeThreadLoop();


	float roll,pitch,yaw;


	char *print_double(char* str, double v, int decimalDigits=2);
#ifdef MAHONY_FILER
	Mahony filter;
#endif
#ifdef MADGWICK_FILTER
	Madgwick filter;
#endif

};


#endif

