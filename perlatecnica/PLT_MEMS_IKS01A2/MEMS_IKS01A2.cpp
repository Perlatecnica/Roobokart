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

#include "mbed.h"
#include "MEMS_IKS01A2.h"

#ifdef CREATE_AS_THREAD
MEMS_IKS01A2::MEMS_IKS01A2() : _thread(){}
#else
MEMS_IKS01A2::MEMS_IKS01A2(){}
#endif

// It starts the class in a thread
void MEMS_IKS01A2::threadStarter(void const *p) {
	MEMS_IKS01A2 *instance = (MEMS_IKS01A2*)p;
	//_thread.signal_set(START_THREAD);
	instance->computeThreadLoop();
}

void MEMS_IKS01A2::init(PinName sda, PinName scl, PinName int1, PinName int2){
	mems_expansion_board = XNucleoIKS01A2::instance(sda, scl, int1, int2);

	magnetometer = mems_expansion_board->magnetometer;
	hum_temp = mems_expansion_board->ht_sensor;
	press_temp = mems_expansion_board->pt_sensor;
	acc_gyro = mems_expansion_board->acc_gyro;
	accelerometer = mems_expansion_board->accelerometer;

	magnetometer->enable();
	hum_temp->enable();
	press_temp->enable();
	acc_gyro->enable_x();
	acc_gyro->enable_g();

	uint8_t id;
	hum_temp->read_id(&id);
	printf("HTS221  humidity & temperature    = 0x%X\r\n", id);
	press_temp->read_id(&id);
	printf("LPS22HB  pressure & temperature   = 0x%X\r\n", id);
	magnetometer->read_id(&id);
	printf("LSM303AGR magnetometer            = 0x%X\r\n", id);
	accelerometer->read_id(&id);
	printf("LSM303AGR accelerometer           = 0x%X\r\n", id);
	acc_gyro->read_id(&id);
	printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

}

//It set Output Data Rate
void MEMS_IKS01A2::setODR(int odr){
	acc_gyro->set_g_odr(odr);
	filter.begin(odr);
}

void MEMS_IKS01A2::computeThreadLoop(){
	//_thread.signal_wait(START_THREAD);
	while(true){
		compute();
	}
}

void MEMS_IKS01A2::compute(){
	// Get new data samples
	acc_gyro->get_x_axes(axesAcc);
	acc_gyro->get_g_axes(axesGyro);

	// The filter library expects gyro data in degrees/s
	float gx = (axesGyro[0]-g_xBias) * 0.001f;
	float gy = (axesGyro[1]-g_yBias) * 0.001f;
	float gz = (axesGyro[2]-g_zBias) * 0.001f;

	float ax = axesAcc[0];
	float ay = axesAcc[1];
	float az = axesAcc[2];

	// Update the filter
	filter.updateIMU(gx, gy, gz, ax, ay, az);

	// Print the orientation filter output
	roll = filter.getRoll();
	pitch = filter.getPitch();
	yaw = filter.getYaw();//+3.23; //Declination correction at Naples 04-2017, 3.23 degrees

	attitude.pitch = pitch;
	attitude.roll = roll;
	attitude.yaw = yaw;

	//printf("Roll: %.2f Pitch: %.2f Yaw: %.2f \n\r",roll,pitch,yaw);
}



LSM303AGRMagSensor *MEMS_IKS01A2::getMagnetometer(){

	return magnetometer;
}

HTS221Sensor *MEMS_IKS01A2::getHumiditySensor(){

	return hum_temp;
}

LPS22HBSensor *MEMS_IKS01A2::getPressureSensor(){

	return press_temp;
}

LSM6DSLSensor *MEMS_IKS01A2::getGyro(){

	return acc_gyro;
}

LSM303AGRAccSensor *MEMS_IKS01A2::getAccelerometer(){
	accelerometer->enable();
	return accelerometer;
}

void MEMS_IKS01A2::calibrateLSM6DSL(int numsamples){
	// Calibrate Gyroscope
	volatile double g_xAccumulator = 0;
	volatile double g_yAccumulator = 0;
	volatile double g_zAccumulator = 0;

	g_xAccumulator = 0;
	g_yAccumulator = 0;
	g_zAccumulator = 0;

	printf("\r\n--- Starting Gyroscope Calibration ---\r\n");

	//Take a number of readings and average them
	//to calculate the gyroscope bias offset.
	for (int i = 0; i < numsamples; i++) {
		acc_gyro->get_g_axes(axesGyro);
		g_xAccumulator += axesGyro[0];
		g_yAccumulator += axesGyro[1];
		g_zAccumulator += axesGyro[2];
		wait_ms(5);
	}

	//Average the samples.
	g_xAccumulator /= numsamples;
	g_yAccumulator /= numsamples;
	g_zAccumulator /= numsamples;

	g_xBias = g_xAccumulator;
	g_yBias = g_yAccumulator;
	g_zBias = g_zAccumulator;

	printf("Gyro bias: %.2f %.2f %.2f \n\r",g_xBias,g_yBias,g_zBias);

	volatile float a_xAccumulator = 0;
	volatile float a_yAccumulator = 0;
	volatile float a_zAccumulator = 0;

	// Calibrate accelerometers inside chip
	printf("\r\n--- Starting Accelerometer Calibration ---\r\n");


	//Take a number of readings and average them
	//to calculate the gyroscope bias offset.
	for (int i = 0; i < numsamples; i++) {
		acc_gyro->get_x_axes(axesAcc);
		a_xAccumulator += (float)axesAcc[0];
		a_yAccumulator += (float)axesAcc[1];
		a_zAccumulator += (float)axesAcc[2];
		wait_ms(5);
	}

	//Average the samples.
	a_xAccumulator /= numsamples;
	a_yAccumulator /= numsamples;
	a_zAccumulator /= numsamples;

	a_xBias = a_xAccumulator;
	a_yBias = a_yAccumulator;
	a_zBias = a_zAccumulator;

	printf("Acc bias: %.2f %.2f %.2f \n\r",a_xBias,a_yBias,a_zBias);
}

/* Helper function for printing floats & doubles */
static char *print_double(char* str, double v, int decimalDigits=2)
{
	int i = 1;
	int intPart, fractPart;
	int len;
	char *ptr;

	/* prepare decimal digits multiplicator */
	for (;decimalDigits!=0; i*=10, decimalDigits--);

	/* calculate integer & fractinal parts */
	intPart = (int)v;
	fractPart = (int)((v-(double)(int)v)*i);

	/* fill in integer part */
	sprintf(str, "%i.", intPart);

	/* prepare fill in of fractional part */
	len = strlen(str);
	ptr = &str[len];

	/* fill in leading fractional zeros */
	for (i/=10;i>1; i/=10, ptr++) {
		if (fractPart >= i) {
			break;
		}
		*ptr = '0';
	}

	/* fill in (rest of) fractional part */
	sprintf(ptr, "%i", fractPart);

	return str;
}
