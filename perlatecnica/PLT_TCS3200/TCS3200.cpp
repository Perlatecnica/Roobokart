#include "TCS3200.h"
#define TCS3200_DELAY 10

TCS3200::TCS3200(const PinName & s0,const PinName & s1,const PinName & s2,const PinName & s3,const PinName & out)
: mS0(s0), mS1(s1), mS2(s2), mS3(s3), signal(out), mode(SCALE_2)
{
    WriteMode(mode);
    signal.rise(callback(this,&TCS3200::HighTrigger));
    signal.fall(callback(this,&TCS3200::LowTrigger));
}

uint32_t TCS3200::ReadColor(const bool & s2, const bool & s3) 
{
    mS2=s2;                    
    mS3=s3;
    ThisThread::sleep_for(TCS3200_DELAY);
    return Fix(pulsewidth);
}
 
void TCS3200::SetMode(const Mode & mode) 
{
    if (this->mode != POWERDOWN) WriteMode(mode);   
    this->mode = mode;
}

void TCS3200::WriteMode(const Mode & mode) 
{
    switch (mode)
    {
        case SCALE_2:
        mS0 = 0;                     
        mS1 = 1;
        break;

        case SCALE_20:
        mS0 = 1;                     
        mS1 = 0;
        break;

        case SCALE_100:
        mS0 = 1;                     
        mS1 = 1;
        break;

        default:
        mS0 = 0;                     
        mS1 = 0;
        break;
    }    
}

void TCS3200::HighTrigger() 
{ 
    timer.start();
}

void TCS3200::LowTrigger() 
{
    timer.stop();
    pulsewidth = timer.read_us();
    timer.reset();
}

uint32_t TCS3200::Fix(const int & value)
{
    if (value >= 0) return (uint32_t)value;
    return 0;
}