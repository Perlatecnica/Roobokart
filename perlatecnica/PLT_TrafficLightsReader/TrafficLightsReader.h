/* Copyright (c) 2022 perlatecnica.it, MIT License
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

/*@author: Francesco Caiazzo */

#ifndef PLT_TRAFFIC_LIGHTS_READER_H
#define PLT_TRAFFIC_LIGHTS_READER_H

#include <mbed.h>
#include "roobokart_def.h"

#if defined(ROOBOKART_V1)
#include "PLT_TCS3200/TCS3200.h"
#elif defined(ROOBOKART_V3)
#include "PLT_VD6283/VD6283.h"
#endif


class TrafficLightsReader
{
    public:
    enum TLR_Info { RED, GREEN, YELLOW, BLACK };

    #if defined(ROOBOKART_V1)
    TrafficLightsReader();
    #elif defined(ROOBOKART_V3)
    TrafficLightsReader(DevI2C * dev);
    #endif

    TLR_Info read();
    inline void start()
    {
		#if defined(ROOBOKART_V1)
		color.Start();
		#endif
    }
    inline void stop()
    {
		#if defined(ROOBOKART_V1)
    	color.Stop();
		#endif
    }

    inline const float & getRed() const { return red; }
    inline const float & getGreen() const { return green; }
    inline const float & getBlue() const { return blue; }
    inline const float & getThreshold() const { return threshold; }
    inline void setThreshold(const float & value) { threshold = value; }

    private:
    void readColors();
    float safeDiv(long val1, long val2);

    private:
    #if defined(ROOBOKART_V1)
    TCS3200 color;
    #elif defined(ROOBOKART_V3)
    VD6283 color;
    #endif
    long clear;
    float red;
    float green;
    float blue;
    float threshold;
};

#endif
