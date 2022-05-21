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
#include "roobokart/roobokart_def.h"
#include "perlatecnica/PLT_TCS3200/TCS3200.h"

class TrafficLightsReader
{
public:
  enum TLR_Info { RED, GREEN, YELLOW, BLACK };

private:
  TCS3200 color;
  long clear;
  float red;
  float green;
  float blue;

public:    
  TrafficLightsReader();  
  TLR_Info read();

  inline void start() { color.SetMode(TCS3200::SCALE_2); }
  inline void stop() { color.SetMode(TCS3200::POWERDOWN); }

  inline const float & getRed() const { return red; }
  inline const float & getGreen() const { return green; }
  inline const float & getBlue() const { return blue; }

private:
  void readColors();
  float safeDiv(long val1, long val2);
};

#endif
