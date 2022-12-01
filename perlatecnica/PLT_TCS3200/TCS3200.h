#ifndef PLT_TCS3200_H
#define PLT_TCS3200_H
 
#include <cstdint>
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

    inline uint32_t ReadRed() { return ReadColor(0, 0); }
    inline uint32_t ReadGreen() { return ReadColor(1, 1); }
    inline uint32_t ReadBlue() { return ReadColor(0, 1); }
    inline uint32_t ReadClear() { return ReadColor(1, 0); }

    void SetMode(const Mode & mode);
    const Mode & GetMode() { return mode; }

    inline void Start() { WriteMode(mode); }
    inline void Stop() { WriteMode(POWERDOWN); }

    private:
    Mode mode;
    DigitalOut mS0;
    DigitalOut mS1;
    DigitalOut mS2;
    DigitalOut mS3;
    InterruptIn signal;
    Timer timer;
    int pulsewidth;
    uint32_t ReadColor(const bool & s2, const bool & s3);
    uint32_t Fix(const int & value);
    void WriteMode(const Mode & mode);
    void HighTrigger();
    void LowTrigger();    
};
 
#endif