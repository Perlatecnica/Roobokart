/**
 ******************************************************************************
 * @file    Stmpe1600.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Header file for component stmpe1600
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
 *       without specific prior written permission.
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
#ifndef     __STMPE1600_CLASS
#define     __STMPE1600_CLASS
/* Includes ------------------------------------------------------------------*/
#include    "DevI2C.h"

#define STMPE1600_DEF_DEVICE_ADDRESS  (uint8_t)0x42*2
#define STMPE1600_DEF_DIGIOUT_LVL      1

#define ORIGINAL_BOARD_CHIP true
#define ORIGINAL_BOARD_CHECK _dev_i2c->i2c_read(data, dev_addr, VERSION_ID,1)==0

/**  STMPE1600 registr map **/
#define CHIP_ID_0_7       (uint8_t)0x00
#define CHIP_ID_8_15      (uint8_t)0x01
#define VERSION_ID        (uint8_t)0x02
#define SYS_CTRL          (uint8_t)0x03
#define IEGPIOR_0_7       (uint8_t)0x08
#define IEGPIOR_8_15	  (uint8_t)0x09
#define ISGPIOR_0_7       (uint8_t)0x0A
#define ISGPIOR_8_15	  (uint8_t)0x0B
#define GPMR_0_7	      (uint8_t)0x10
#define GPMR_8_15	      (uint8_t)0x11
#define GPSR_0_7	      (uint8_t)0x12
#define GPSR_8_15	      (uint8_t)0x13
#define GPDR_0_7	      (uint8_t)0x14
#define GPDR_8_15	      (uint8_t)0x15
#define GPIR_0_7	      (uint8_t)0x16
#define GPIR_8_15	      (uint8_t)0x17

#define SOFT_RESET        (uint8_t)0x80

/**  MAX7311 registr map **/
#define IP1ADDR	(uint8_t) 0x00	// input port 1					xxxx xxxx
#define IP2ADDR	(uint8_t) 0x01	// input port 2					xxxx xxxx
#define OP1ADDR	(uint8_t) 0x02	// output port 1				1111 1111
#define OP2ADDR	(uint8_t) 0x03	// output port 2				1111 1111
#define P1PADDR	(uint8_t) 0x04	// port 1 polarity inversion	0000 0000
#define P2PADDR	(uint8_t) 0x05	// port 2 polarity inversion	0000 0000
#define P1CADRR	(uint8_t) 0x06	// port 1 configuration			1111 1111
#define P2CADRR	(uint8_t) 0x07	// port 2 configuration			1111 1111
#define TORADRR	(uint8_t) 0x08	// timeout register				0000 0000

typedef enum {
    // GPIO Expander pin names
    GPIO_0 = 0,
    GPIO_1,
    GPIO_2,
    GPIO_3,
    GPIO_4,
    GPIO_5,
    GPIO_6,
    GPIO_7,
    GPIO_8,
    GPIO_9,
    GPIO_10,
    GPIO_11,
    GPIO_12,
    GPIO_13,
    GPIO_14,
    GPIO_15,
    NOT_CON
} ExpGpioPinName;

typedef enum {
    INPUT = 0,
    OUTPUT,
    NOT_CONNECTED
} ExpGpioPinDirection;

/**  STMPE1600 enum **/
/*enum class STMPE1600_ExpGpioPinDirection {
    INPUT = 0,
    OUTPUT,
    NOT_CONNECTED
};*/

/**  MAX7311 enum **/
/*enum class MAX7311_ExpGpioPinDirection {
	OUTPUT = 0,
    INPUT,
    NOT_CONNECTED
};*/

/* Classes -------------------------------------------------------------------*/
/** Class representing a single stmpe1600 GPIO expander output pin
 */
class Stmpe1600DigiOut
{
private:
    DevI2C *_dev_i2c;
    uint8_t exp_dev_addr;
    ExpGpioPinName exp_pin_name;
	bool STMPE1600_board = ORIGINAL_BOARD_CHIP; // check if the original board is available

public:
    /** Constructor
     * @param[in] &i2c device I2C to be used for communication
     * @param[in] outpinname the desired out pin name to be created
     * @param[in] DevAddr the stmpe1600 I2C device address (deft STMPE1600_DEF_DEVICE_ADDRESS)
     * @param[in] lvl the default ot pin level
     */

    Stmpe1600DigiOut(DevI2C &i2c, ExpGpioPinName out_pin_name, uint8_t dev_addr = STMPE1600_DEF_DEVICE_ADDRESS,
                     bool lvl = STMPE1600_DEF_DIGIOUT_LVL) : _dev_i2c(&i2c), exp_pin_name(out_pin_name)
    {
        uint8_t data[2];

        if (exp_pin_name == NOT_CON) {
            return;
        }


        STMPE1600_board = ORIGINAL_BOARD_CHECK;

        /* set the exp_pin_name as output */

        if (STMPE1600_board) // STMPE1600
        {
        	exp_dev_addr = dev_addr;

        	/* read configs */
			_dev_i2c->i2c_read(data, exp_dev_addr, GPDR_0_7, 1);
			_dev_i2c->i2c_read(&data[1], exp_dev_addr, GPDR_8_15, 1);

			/* set gpio as out */
			*(uint16_t *) data = * (uint16_t *) data | (1 << (uint16_t) exp_pin_name);

			/* write configs */
			_dev_i2c->i2c_write(data, exp_dev_addr, GPDR_0_7, 1);
			_dev_i2c->i2c_write(&data[1], exp_dev_addr, GPDR_8_15, 1);
        }
        else // MAX7311
        {
        	exp_dev_addr = dev_addr-68;

        	/* read configs */
			_dev_i2c->i2c_read(data, exp_dev_addr, P1CADRR, 1);
			_dev_i2c->i2c_read(&data[1], exp_dev_addr, P2CADRR, 1);

			/* set gpio as out */
			*(uint16_t *) data = * (uint16_t *)data & (uint16_t)(~(1 << (uint16_t) exp_pin_name));

			/* write configs */
			_dev_i2c->i2c_write(data, exp_dev_addr, P1CADRR, 1);
			_dev_i2c->i2c_write(&data[1], exp_dev_addr, P2CADRR, 1);
        }

        write(lvl);
    }

    /**
     * @brief       Write on the out pin
     * @param[in]   lvl level to write
     * @return      0 on Success
     */
    void write(int lvl)
    {
        uint8_t data[2];

        if (exp_pin_name == NOT_CON) {
            return;
        }

        if (STMPE1600_board)
        {
			/* set the exp_pin_name state to lvl */
			_dev_i2c->i2c_read(data, exp_dev_addr, GPSR_0_7, 2);
			* (uint16_t *) data = * (uint16_t *) data & (uint16_t)(~(1 << (uint16_t) exp_pin_name));               // set pin mask
			if (lvl) {
				* (uint16_t *) data = * (uint16_t *) data | (uint16_t)(1 << (uint16_t) exp_pin_name);
			}
			_dev_i2c->i2c_write(data, exp_dev_addr, GPSR_0_7, 2);
        }
        else
        {
        	/* set the exp_pin_name state to lvl */

			// read output
			_dev_i2c->i2c_read(data, exp_dev_addr, OP1ADDR, 2);

			// clear bit
			* (uint16_t *) data = * (uint16_t *) data & (uint16_t)(~(1 << (uint16_t) exp_pin_name));               // set pin mask

			// if true set bit
			if (lvl) {
				* (uint16_t *) data = * (uint16_t *) data | (uint16_t)(1 << (uint16_t) exp_pin_name);
			}

			// write output
			_dev_i2c->i2c_write(data, exp_dev_addr, OP1ADDR, 2);
        }
    }

    /**
     * @brief       Overload assignement operator
     */
    Stmpe1600DigiOut &operator= (int lvl)
    {
        write(lvl);
        return *this;
    }
};

/* Classes -------------------------------------------------------------------*/
/** Class representing a single stmpe1600 GPIO expander input pin
 */
class Stmpe1600DigiIn
{
private:
	DevI2C *_dev_i2c;
	uint8_t exp_dev_addr;
	ExpGpioPinName exp_pin_name;
	bool STMPE1600_board = ORIGINAL_BOARD_CHIP; // check if the original board is available

public:
    /** Constructor
    * @param[in] &i2c device I2C to be used for communication
    * @param[in] inpinname the desired input pin name to be created
    * @param[in] DevAddr the stmpe1600 I2C device addres (deft STMPE1600_DEF_DEVICE_ADDRESS)
    */
    Stmpe1600DigiIn(DevI2C &i2c, ExpGpioPinName in_pin_name, uint8_t dev_addr = STMPE1600_DEF_DEVICE_ADDRESS) : _dev_i2c(&i2c), exp_pin_name(in_pin_name)
    {
        uint8_t data[2];

        if (exp_pin_name == NOT_CON) {
            return;
        }

        STMPE1600_board = ORIGINAL_BOARD_CHECK;

        if (STMPE1600_board)
        {
        	exp_dev_addr = dev_addr;
			/* set the exp_pin_name as input pin direction */
			_dev_i2c->i2c_read(data, exp_dev_addr, GPDR_0_7, 2);
			* (uint16_t *) data = * (uint16_t *) data & (uint16_t)(~(1 << (uint16_t) exp_pin_name)); // set gpio as in
			_dev_i2c->i2c_write(data, exp_dev_addr, GPDR_0_7, 2);
        }
        else
        {
        	exp_dev_addr = dev_addr-68;
        	/* set the exp_pin_name as input pin direction */
			// read config
			_dev_i2c->i2c_read(data, exp_dev_addr, P1CADRR, 2);
			* (uint16_t *) data = * (uint16_t *) data | (1 << (uint16_t) exp_pin_name);	// set gpio as in
			// write config
			_dev_i2c->i2c_write(data, exp_dev_addr, P1CADRR, 2);
        }
    }

    /**
     * @brief       Read the input pin
     * @return      The pin logical state 0 or 1
     */
    bool read()
    {
        uint8_t data[2];

        if (exp_pin_name == NOT_CON) {
            return false;
        }

        if (STMPE1600_board)
        {
			/* read the exp_pin_name */
			_dev_i2c->i2c_read(data, exp_dev_addr, GPMR_0_7, 2);
			* (uint16_t *) data = * (uint16_t *) data & (uint16_t)(1 << (uint16_t) exp_pin_name);            // mask the in gpio
			if (data[0] || data[1]) {
				return true;
			}
        }
        else
        {
        	/* read the exp_pin_name */
			_dev_i2c->i2c_read(data, exp_dev_addr, IP1ADDR, 2);
			* (uint16_t *) data = * (uint16_t *) data & (uint16_t)(1 << (uint16_t) exp_pin_name);            // mask the in gpio
			if (data[0] || data[1]) {
				return true;
			}
        }
        return false;
    }

    operator int()
    {
        return read();
    }

};

/* Classes -------------------------------------------------------------------*/
/** Class representing a whole stmpe1600 component (16 gpio)
 */
class Stmpe1600
{
private:
    DevI2C *_dev_i2c;
    uint16_t gpdr0_15;  // local copy of bit direction reg
    uint16_t gpsr0_15;  // local copy of bit status reg
    uint8_t exp_dev_addr; // expander device i2c addr
    bool STMPE1600_board = ORIGINAL_BOARD_CHIP; // check if the original board is available

public:
    /** Constructor
    * @param[in] &i2c device I2C to be used for communication
    * @param[in] DevAddr the stmpe1600 I2C device addres (deft STMPE1600_DEF_DEVICE_ADDRESS)
    */
    Stmpe1600(DevI2C &i2c, uint8_t dev_addr = STMPE1600_DEF_DEVICE_ADDRESS) : _dev_i2c(&i2c)
    {
    	uint8_t data[2];
    	STMPE1600_board = ORIGINAL_BOARD_CHECK;
    	if (STMPE1600_board)
    	{
			exp_dev_addr = dev_addr;
			write_sys_ctrl(SOFT_RESET);
			gpdr0_15 = (uint16_t) 0;	// gpio dir all IN
			write_16bit_reg(GPDR_0_7, &gpdr0_15);
			gpsr0_15 = (uint16_t) 0x0ffff;   // gpio status all 1
			write_16bit_reg(GPSR_0_7, &gpsr0_15);
    	}
    	else
    	{
    		exp_dev_addr = dev_addr - 68;
			//write_sys_ctrl(SOFT_RESET);

			gpdr0_15 = (uint16_t) 0xffff;	// gpio dir all IN
			write_16bit_reg(P1CADRR, &gpdr0_15);

			gpsr0_15 = (uint16_t) 0xffff;   // gpio status all 1
			write_16bit_reg(OP1ADDR, &gpsr0_15);
    	}
    }

    /**
     * @brief       Write the SYS_CTRL register
     * @param[in]   Data to be written (bit fields)
     */
    void write_sys_ctrl(uint8_t data)      // data = SOFT_RESET reset the device
    {
    	if (STMPE1600_board)
    	{
    		_dev_i2c->i2c_write(&data, exp_dev_addr, SYS_CTRL, 1);
    	}
    }

    /**
     * @brief       Set the out pin
     * @param[in]   The pin name
     * @return      0 on Success
     */
    bool set_gpio(ExpGpioPinName pin_name)
    {
        if (pin_name == NOT_CON) {
            return true;
        }

        gpsr0_15 = gpsr0_15 | ((uint16_t) 0x0001 << pin_name);

        if (STMPE1600_board)
        {
			write_16bit_reg(GPSR_0_7, &gpsr0_15);
        }
        else
        {

			write_16bit_reg(OP1ADDR, &gpsr0_15);
        }

        return false;
    }

    /**
     * @brief       Clear the out pin
     * @param[in]   The pin name
     * @return      0 on Success
     */
    bool clear_gpio(ExpGpioPinName pin_name)
    {
        if (pin_name == NOT_CON) {
            return true;
        }
        gpsr0_15 = gpsr0_15 & (~((uint16_t) 0x0001 << pin_name));

        if (STMPE1600_board)
        {
			write_16bit_reg(GPSR_0_7, &gpsr0_15);
        }
        else
        {
        	write_16bit_reg(OP1ADDR, &gpsr0_15);
        }
        return false;
    }

    /**
     * @brief       Read the input pin
     * @param[in]   The pin name
     * @return      The logical pin level
     */
    bool read_gpio(ExpGpioPinName pin_name)
    {
        uint16_t gpmr0_15;
        if (pin_name == NOT_CON) {
            return true;
        }

        if(STMPE1600_board)
        {
        	read_16bit_reg(GPMR_0_7, &gpmr0_15);
        }
        else
        {
        	read_16bit_reg(IP1ADDR, &gpmr0_15);

        }
        gpmr0_15 = gpmr0_15 & ((uint16_t) 0x0001 << pin_name);

        if (gpmr0_15) {
            return true;
        }
        return false;
    }

    /**
     * @brief       Set the pin direction
     * @param[in]   The pin name
     * @param[in]   The pin direction
     * @return      0 on success
     */
    bool set_gpio_dir(ExpGpioPinName pin_name, ExpGpioPinDirection pin_dir)
    {
    	if (pin_name == NOT_CON || pin_dir == NOT_CONNECTED) { return true; }
        if (STMPE1600_board)
        {

			gpdr0_15 = gpdr0_15 & (~((uint16_t) 0x0001 << pin_name));        // clear the Pin
			gpdr0_15 = gpdr0_15 | ((uint16_t) pin_dir << pin_name);
			write_16bit_reg(GPDR_0_7, &gpdr0_15);
        }
        else
        {
        	if (pin_dir == INPUT) { pin_dir = OUTPUT; }
        	else { pin_dir = INPUT; }
			gpdr0_15 = gpdr0_15 & (~((uint16_t) 0x0001 << pin_name));        // clear the Pin
			gpdr0_15 = gpdr0_15 | ((uint16_t) pin_dir << pin_name);
			write_16bit_reg(P1CADRR, &gpdr0_15);
        }
        return false;
    }

    /**
     * @brief       Read a 16 bits register
     * @param[in]   The register address
     * @param[in]   The pointer to the read data
     */
    void read_16bit_reg(uint8_t reg16_addr, uint16_t *reg16_data)
    {
    	if (STMPE1600_board)
    	{
    		_dev_i2c->i2c_read((uint8_t *) reg16_data, exp_dev_addr, reg16_addr, 2);
    	}
    	else
    	{
    		reg16_addr = stmpe1600ToMax7311Reg(reg16_addr);
			_dev_i2c->i2c_read((uint8_t *) reg16_data, exp_dev_addr, reg16_addr, 2);
			stmpe1600ToMax7311Config(reg16_addr, reg16_data);
    	}
    }

    /**
     * @brief       Write a 16 bits register
     * @param[in]   The register address
     * @param[in]   The pointer to the data to be written
     */
    void write_16bit_reg(uint8_t reg16_addr, uint16_t *reg16_data)
    {
    	if (STMPE1600_board)
    	{
    		_dev_i2c->i2c_write((uint8_t *) reg16_data, exp_dev_addr, reg16_addr, 2);
    	}
    	else
    	{
    		reg16_addr = stmpe1600ToMax7311Reg(reg16_addr);
			stmpe1600ToMax7311Config(reg16_addr, reg16_data);
			_dev_i2c->i2c_write((uint8_t *) reg16_data, exp_dev_addr, reg16_addr, 2);
    	}
    }

private:
	uint8_t stmpe1600ToMax7311Reg(uint8_t reg16_addr)
	{
		switch (reg16_addr)
		{
			// GPMR
			case 0x10: return IP1ADDR;
			case 0x11: return IP2ADDR;

			// GPSR
			case 0x12: return OP1ADDR;
			case 0x13: return OP2ADDR;

			// GPDR
			case 0x14: return P1CADRR;
			case 0x15: return P2CADRR;

			default:
			break;
		}

		return reg16_addr;
	}

	void stmpe1600ToMax7311Config(uint8_t reg16_addr, uint16_t *reg16_data)
	{
		switch (reg16_addr)
		{
			case P1CADRR:
			case P2CADRR:
			(*reg16_data) = (uint16_t)(~(*reg16_data));
			break;

			default:
			break;
		}
	}
};

#endif // __STMPE1600_CLASS
