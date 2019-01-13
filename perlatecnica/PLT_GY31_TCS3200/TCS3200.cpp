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

#include "TCS3200.h"
#include "mbed.h"
 
 
TCS3200::TCS3200(PinName S0, PinName S1, PinName S2, PinName S3, PinName OUT) :
mS0(S0), mS1(S1), mS2(S2), mS3(S3), signal(OUT) 
{
    SetMode(POWERDOWN);
    signal.rise(this,&TCS3200::HighTrigger);
    signal.fall(this,&TCS3200::LowTrigger);
}
 
long TCS3200::ReadRed() {
    mS2=0;                    
    mS3=0;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}

long TCS3200::ReadGreen() {
    mS2=1;                    
    mS3=1;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}

long TCS3200::ReadBlue() {
    mS2=0;                    
    mS3=1;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}

long TCS3200::ReadClear() {
    mS2=1;                    
    mS3=0;
    wait(0.1);     //Delay to allow frequency to change for the set color
    return(pulsewidth);
}

void TCS3200::SetMode(TCS3200Mode mode) {
    if(mode == POWERDOWN) {         //TCS3200 in power down
        mS0 = 0;                     
        mS1 = 0;
    }
    else if(mode == SCALE_2) {      //Output frequency at 2% scaling
        mS0 = 0;                     
        mS1 = 1;
    }
    else if(mode == SCALE_20) {     //Output frequency at 20% scaling
        mS0 = 1;                     
        mS1 = 0;
    }
    else if(mode == SCALE_100) {    //Output frequency at 100% scaling
        mS0 = 1;                     
        mS1 = 1;
    }
    else {                          //default is POWERDOWN                
        mS0 = 0;                     
        mS1 = 0;
    }
}

void TCS3200::HighTrigger() {
    timer.start();
}

void TCS3200::LowTrigger() {
    timer.stop();
    pulsewidth = timer.read_us();
    timer.reset();
}
