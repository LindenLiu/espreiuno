/**
 * MIT License
 *
 * Copyright (c) 2022 Linden Liu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once
#include <PressureTransducer.h>

class XDBPressureTransducer : public PressureTransducer
{
private:
  /* data */
  // The bar range is 0 to 12 bar, the voltage range is 0.5 to 4.5, the adc is 10 bit [0, 1024). 
  // So the bar per voltage is (barMax - barMin) / Vmax - Vmin. Reading per V is 1024 / 5.
  // So the bar per adc reading 1 is (barMax - barMin) / (Vmax - Vmin) / (1024 / 5)
  const float aToBar = 15.0/1024.0;
  const float barStart = 0.5 * 3 ; // The output voltage is 0.5 to 4.5v, bar per v is 3.

  unsigned char pressurePin;  
public:
  XDBPressureTransducer(unsigned char pressurePin);
  ~XDBPressureTransducer();
  void begin();
  float getPressure() override;
};

XDBPressureTransducer::XDBPressureTransducer(unsigned char pressurePin): pressurePin(pressurePin)
{
}

XDBPressureTransducer::~XDBPressureTransducer()
{
}

void XDBPressureTransducer::begin() 
{
  pinMode(this->pressurePin, INPUT);
}

float XDBPressureTransducer::getPressure() {  
    return analogRead(pressurePin)*aToBar - barStart; // converting to bars
}
