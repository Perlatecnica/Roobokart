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

#include "MotorShieldIHM12A1.h"

#include "../../roobopoli/roobokart/roobokart_def.h"
#include "mbed.h"
 
STSpin240_250_init_t init =
 {
  20000, /* Frequency of PWM of Input Bridge A in Hz up to 100000Hz             */
  20000, /* Frequency of PWM of Input Bridge B in Hz up to 100000Hz             */
  20000, /* Frequency of PWM used for Ref pin in Hz up to 100000Hz              */
  50,    /* Duty cycle of PWM used for Ref pin (from 0 to 100)                  */
  TRUE   /* Dual Bridge configuration  (FALSE for mono, TRUE for dual brush dc) */
 };

/* Motor Control Component. */
STSpin240_250 *motor;

 
void my_error_handler(uint16_t error)
{
  /* Printing to the console. */
  printf("Error %d detected\r\n\n", error);
  
  /* Infinite loop */
  while (true) {
  }    
}

/**
 * @brief  This is an example of user handler for the flag interrupt.
 * @param  None
 * @retval None
 * @note   If needed, implement it, and then attach and enable it:
 *           + motor->attach_flag_irq(&my_flag_irq_handler);
 *           + motor->enable_flag_irq();
 *         To disable it:
 *           + motor->DisbleFlagIRQ();
 */
void my_flag_irq_handler(void)
{
   /* Code to be customised */
  /************************/

  printf("    WARNING: \"FLAG\" interrupt triggered.\r\n");

  /* Get the state of bridge A */
  uint16_t bridgeState  = motor->get_bridge_status(0);
  
  if (bridgeState == 0)  {
    if (motor->get_device_state(0) != INACTIVE) {
      /* Bridges were disabled due to overcurrent or over temperature */
      /* When  motor was running */
      my_error_handler(0XBAD0);
    }
  }
}
 
MotorShieldIHM12A1::MotorShieldIHM12A1()
{
    
//----- Initialization 
  
  /* Initializing Motor Control Component. */
  /*#if (defined TARGET_NUCLEO_F030R8)||(defined TARGET_NUCLEO_F334R8)
  motor = new STSPIN240_250(D2, D9, D6, D7, D5, D4, A2);
  #elif (defined TARGET_NUCLEO_L152RE)
  motor = new STSPIN240_250(D2, D9, D6, D7, D5, D4, A3);
  #else*/
  printf("Motor Control Creating Instance\r\n");
  motor = new STSpin240_250(F_E_PIN, STANDBY_RST_PIN, DIR_A_PIN, DIR_B_PIN, PWM_A_PIN, PWM_B_PIN, PWM_REF_PIN);
  printf("Motor Control Instance Created \r\n");
  //#endif
  if (motor->init(&init) != COMPONENT_OK) exit(EXIT_FAILURE);
  else printf("Motor Control Init passed  \r\n");
  
  /* Set dual bridge enabled as two motors are used*/
  motor->set_dual_full_bridge_config(1);
  printf("Motor Control dual full bridge \r\n");
  
  /* Attaching and enabling an interrupt handler. */
    //motor->attach_flag_irq(&my_flag_irq_handler);
  //motor->enable_flag_irq();
  printf("Motor Control attach_flag_irq \r\n");
  
    
  /* Attaching an error handler */
  motor->attach_error_handler(&my_error_handler);

  /* Printing to the console. */
  printf("Motor Control Application Example for 2 brush DC motors\r\n");

  /* Set PWM Frequency of Ref to 15000 Hz */ 
  motor->set_ref_pwm_freq(0, 15000); 

  /* Set PWM duty cycle of Ref to 60% */ 
  motor->set_ref_pwm_dc(0, 60); 
  
  /* Set PWM Frequency of bridge A inputs to 10000 Hz */ 
  motor->set_bridge_input_pwm_freq(0,10000); 
  
  /* Set PWM Frequency of bridge B inputs to 10000 Hz */ 
  motor->set_bridge_input_pwm_freq(1,10000);  
  
}
 
void MotorShieldIHM12A1::speed(unsigned int ID, int8_t velocity)
{ 
    if  (velocity > 100)   velocity = 100;
    else if (velocity < -100)  velocity = -100;
      
    if (velocity>0)
    {
        motor->set_speed(ID,velocity); 
        motor->run(ID, BDCMotor::FWD);
    }
    else if (velocity<0)
    {
        motor->set_speed(ID,-velocity); 
        motor->run(ID, BDCMotor::BWD);
    }
    else
    {
        motor->hard_stop(ID);
    } 
}
 
void MotorShieldIHM12A1::stop(unsigned int ID)
{
    motor->hard_hiz(ID);       
}

void MotorShieldIHM12A1::stop()
{
    motor->hard_hiz(0);
    motor->hard_hiz(1);
}
 
void MotorShieldIHM12A1::reset()
{
    motor->reset();
}

void MotorShieldIHM12A1::saturate(int8_t *value)
{
	if(*value>MAX_SPEED_VALUE)
	{
		*value = MAX_SPEED_VALUE;
	}
	if(*value<-MAX_SPEED_VALUE)
	{
		*value = -MAX_SPEED_VALUE;
	}
}

void MotorShieldIHM12A1::turn(int8_t direction, int8_t velocity, unsigned int lMotor, unsigned int rMotor){
	int8_t leftMotspeed;
	int8_t rigthMotspeed;
	saturate(&direction);
	saturate(&velocity);

	// direction > 0 => Turn on the right
	if(direction > 0){
		if(velocity + direction <= MAX_SPEED_VALUE){
			leftMotspeed = (velocity + direction);
			rigthMotspeed = (velocity);
		} else {
			leftMotspeed = (MAX_SPEED_VALUE);
			rigthMotspeed = (MAX_SPEED_VALUE - direction);
		}

	}
	// direction < 0 => Turn on the left
	else if(direction < 0){
		direction = abs(direction);
		if(velocity + direction <= MAX_SPEED_VALUE){
			rigthMotspeed = (velocity + direction);
			leftMotspeed = (velocity);
		} else {
			rigthMotspeed = (MAX_SPEED_VALUE);
			leftMotspeed = (MAX_SPEED_VALUE - direction);
		}
	}
	// direction = 0
	else{
		leftMotspeed = (velocity);
		rigthMotspeed = (velocity);
	}


	speed(rMotor,rigthMotspeed);
	speed(lMotor,leftMotspeed);
}

void MotorShieldIHM12A1::follow(int8_t direction, int8_t velocity, unsigned int lMotor, unsigned int rMotor)
{
	int8_t lM;
	int8_t rM;
	saturate(&direction);
	saturate(&velocity);

	lM = (+direction/2+velocity);
	rM = (-direction/2+velocity);

	saturate(&lM);
	saturate(&rM);

	speed(rMotor,lM);
	speed(lMotor,rM);
}

void MotorShieldIHM12A1::run(int8_t direction, int8_t velocity, unsigned int lMotor, unsigned int rMotor)
{
    bool left = false;
    float speedMod;
    
    if (direction < 0)
    {
        left = true;
        direction = -direction;
    }
 
    speedMod = ((float)velocity/100)*(direction-50)*(-2);
    
    if (left)
    {
        
        speed(rMotor,speedMod);
        speed(lMotor,velocity);
    }
    else
    {
        speed(lMotor,speedMod);
        speed(rMotor,velocity);
    }
}

