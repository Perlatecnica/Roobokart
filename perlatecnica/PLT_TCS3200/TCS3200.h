#ifndef PLT_TCS3200_H
#define PLT_TCS3200_H
 
#include <mbed.h>

class TCS3200 
{
    public:
    enum Mode 
    {
        POWERDOWN = 0,
        SCALE_2 = 1,
        SCALE_20 = 2,
        SCALE_100 = 3,
    };   

    TCS3200() = delete;
    TCS3200(const PinName & s0,const PinName & s1,const PinName & s2,const PinName & s3,const PinName & out);
    TCS3200(const TCS3200 &) = delete;
    TCS3200(TCS3200 &&) = delete;

    TCS3200& operator=(const TCS3200 &) = delete;
    TCS3200& operator=(TCS3200 &&) = delete;

    long ReadRed();    
    long ReadGreen();
    long ReadBlue();    
    long ReadClear();    
    void SetMode(Mode mode);

    private:
    DigitalOut mS0;
    DigitalOut mS1;
    DigitalOut mS2;
    DigitalOut mS3;
    InterruptIn signal;
    Timer timer;
    long pulsewidth;
    void HighTrigger();
    void LowTrigger();    
};
 
#endif