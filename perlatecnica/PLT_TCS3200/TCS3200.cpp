#include "TCS3200.h"
#define TCS3200_DELAY 10

TCS3200::TCS3200(const PinName & s0,const PinName & s1,const PinName & s2,const PinName & s3,const PinName & out)
: mS0(s0), mS1(s1), mS2(s2), mS3(s3), signal(out) 
{
    SetMode(POWERDOWN);
    signal.rise(callback(this,&TCS3200::HighTrigger));
    signal.fall(callback(this,&TCS3200::LowTrigger));
}
 
long TCS3200::ReadRed() 
{
    mS2=0;                    
    mS3=0;
    ThisThread::sleep_for(TCS3200_DELAY);
    return(pulsewidth);
}
 
long TCS3200::ReadGreen() 
{
    mS2=1;                    
    mS3=1;
    ThisThread::sleep_for(TCS3200_DELAY);
    return(pulsewidth);
}
 
long TCS3200::ReadBlue() 
{
    mS2=0;                    
    mS3=1;
    ThisThread::sleep_for(TCS3200_DELAY);
    return(pulsewidth);
}
 
long TCS3200::ReadClear() 
{
    mS2=1;                    
    mS3=0;
    ThisThread::sleep_for(TCS3200_DELAY);
    return(pulsewidth);
}
 
void TCS3200::SetMode(Mode mode) 
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