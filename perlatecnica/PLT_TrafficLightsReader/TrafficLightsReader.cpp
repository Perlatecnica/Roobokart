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

#include "TrafficLightsReader.h"

#if defined(ROOBOKART_V1)
TrafficLightsReader::TrafficLightsReader()
: color(TCS3200_S0, TCS3200_S1, TCS3200_S2, TCS3200_S3, TCS3200_OUT), threshold(COLOR_THRESHOLD)
{
    color.SetMode(TCS3200::SCALE_2);
    color.Stop();
}
#elif defined(ROOBOKART_V3)
TrafficLightsReader::TrafficLightsReader(DevI2C * dev)
: color(dev), threshold(COLOR_THRESHOLD)
{
	#if defined(ROOBOKART_V1)
    color.Stop();
	#endif
    color.init();
}
#endif

TrafficLightsReader::TLR_Info TrafficLightsReader::read()
{
    readColors();

    if (abs(red - green) > threshold)
    {
        if ((red > green) && (red > blue)) return RED;
        else if ((green > blue) && (green > red)) return GREEN;
    }
    else
    {
        if ((red - blue) > threshold) return YELLOW;
    }
    return BLACK;
}

void TrafficLightsReader::readColors()
{
    #if defined(ROOBOKART_V1)
    clear = color.ReadClear();
    green = safeDiv(clear, color.ReadGreen());
    blue = safeDiv(clear, color.ReadBlue());
    red = safeDiv(clear, color.ReadRed());
    #elif defined(ROOBOKART_V3)
    color.updateValues();
    clear = color.getVisible();
    green = safeDiv(color.getGreen(),clear);
    blue = safeDiv(color.getBlue(),clear);
    red = safeDiv(color.getRed(),clear);
    #endif
}

float TrafficLightsReader::safeDiv(long val1, long val2)
{
    if (val2 == 0) return 0.0f;
    return (float) val1 / val2;
}
