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

TrafficLightsReader::TrafficLightsReader()
: color(TCS3200_S0, TCS3200_S1, TCS3200_S2, TCS3200_S3, TCS3200_OUT) {}

TrafficLightsReader::TLR_Info TrafficLightsReader::read()
{
  readColors(); 
    
  if (abs(red - green) > COLOR_THRESHOLD)
  {
    if ((red > green) && (red > blue))
    {
      return RED;
    }
    else if ((green > blue) && (green > red))
    {
      return GREEN;
    }
    else
    {
      return BLACK;
    }
  } 
  else 
  {
    if ((red - blue) > COLOR_THRESHOLD) return YELLOW;
    else return BLACK;
  }
}

void TrafficLightsReader::readColors()
{
  clear = color.ReadClear();
  red = safeDiv(clear, color.ReadRed());
  green = safeDiv(clear, color.ReadGreen());
  blue = safeDiv(clear, color.ReadBlue());
  printf("%6.4f %6.4f %6.4f\r\n", red, green, blue);
}

float TrafficLightsReader::safeDiv(long val1, long val2)
{
  if (val2 == 0) return 0.0f;
  return (float) val1 / val2;
}
