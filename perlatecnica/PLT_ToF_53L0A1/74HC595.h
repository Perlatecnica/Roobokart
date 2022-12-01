#ifndef SHIFT_REGISTER_74HC595_H
#define SHIFT_REGISTER_74HC595_H

#include "mbed.h"

class SR74HC595
{
    uint8_t data; 
    int freq;
    SPI spi;
    DigitalOut cs;
    //Mutex mutex;

public:

    typedef enum : uint8_t {
        GPIO_0 = 0b00000001,
        GPIO_1 = 0b00000010,
        GPIO_2 = 0b00000100,
        GPIO_3 = 0b00001000,
        GPIO_4 = 0b00010000,
        GPIO_5 = 0b00100000,
        GPIO_6 = 0b01000000,
        GPIO_7 = 0b10000000
    } GPIO;

    SR74HC595() = delete;
    SR74HC595(PinName mosi, PinName miso, PinName sck, PinName spi_cs, int frequency = 42000000)
    : spi(mosi, miso, sck), cs(spi_cs, 0), freq(frequency), data(0)
    {
    	spi.format(8,3);
    	spi.frequency(freq);
        setAll(0b00000000);
    }

    int setAll(const uint8_t &value)
    {
        int error;
        spi.lock();
        data = value;        
        error = spiThreadUnsafeSingleWrite(value);
        spi.unlock();
        return error;       
    }

    int setSingle(const bool &value, const GPIO &gpio)
    {
        uint8_t temp;
        int error;
        spi.lock();        
        if (value) temp = data | gpio;
        else temp =  data & (~gpio);
        data = temp;        
        error = spiThreadUnsafeSingleWrite(data);
        spi.unlock();
        return error;    
    }

private:

    int spiThreadUnsafeSingleWrite(const uint8_t &value)
    {
        int error;
        cs.write(0);
        error = spi.write(value);
        cs.write(1);
        return error;
    }      
};

class SR74HC595DigitalOut
{
    SR74HC595::GPIO mask;
    SR74HC595 * sr;

public: 

    SR74HC595DigitalOut() = delete;
    SR74HC595DigitalOut(SR74HC595 * shiftRegister74HC595, const SR74HC595::GPIO &gpio, const int &def_value = 0) : sr(shiftRegister74HC595), mask(gpio)
    {
        write(def_value);
    }

    int write(const bool &value)
    {      
        return sr->setSingle(value, mask);
    }    

private:

};

#endif
