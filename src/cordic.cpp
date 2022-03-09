/*

   File: cordic.c
   Purpose: CORDIC based trigonometric functions library for Arduino
   Version: 1.0.0
   Release Date: 28-06-2019


   URL: https://github.com/MartinStokroos/fastCORDIC

   MIT License

   Copyright (c) 2019 Martin Stokroos

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

*/


/* This library is a collection (under construction) of trigonometric functions, efficiently
   solved with the COordinate Rotation DIgital Computer (CORDIC) algorithm.
   The function implemented here is the atan2 function, which calculates the angle and
   radius of the vector that points to (x,y). This function is very useful in DSP
   applications such as digital down conversion.

   The CORDIC implementation of ATAN2 is faster than ATAN2 from the standard library.
   The CORDIC algorithm rotates a vector until it is aligned with the positive x-axis.
   In this implementation the length of the vector is retained and thus, the square-root
   of x^2+y^2 is known and equals to x0 at the function exit.

   The atan2sqrt function presented here is based on the CORDIC method that was provided with
   the course material about 'Software Defined Radio with AVR' [Ossmann 2012].
   The value of x0 and y0 (the rotated vector) should stay within the size of an int.
   Furthermore an atan2sqrt function with 10-bit phase resolution has been added.

   Reference:
   Software Defined Radio met AVR, Deel 4: Digitale radio-ontvangst,
   DCF77, weerdienst en meer..., Martin Ossmann, Elektor(NL) June 2012.

*/


#include "cordic.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


/*
   8-bit CORDIC atan2sqrt
   Calculates the angle and radius of the vector pointing towards (x,y)
   The angle is returned as an unsigned char. 256 means 2pi or 360 degrees.
   radius is the length of the vector as an int.
*/
void cordic8::atan2sqrt(int _x, int _y) {   //max input values for x and y must be confirmed.
  x0 = _x; y0 = _y; angle = 0; sign = false;
  if (y0 < 0) {
    x0 = -x0;  //rotate 128 cw
    y0 = -y0;
    angle = 128;
  }
  if (x0 < 0) {
    tmp = x0;  //rotate 64 cw
    x0 = y0;
    y0 = -tmp;
    angle += 64;
  }
  if (y0 > x0) {
    sign = true;
    angle += 64;
    tmp = x0;
    x0 = y0;
    y0 = tmp;
  }
  currentAngle = 16; phi = 0; k = 0;
  while (k <= 4) {
    x1 = (long)cos_lut[k] * (long)x0;
    x1 += (long)sin_lut[k] * (long)y0;
    y1 = -(long)sin_lut[k] * (long)x0;
    y1 += (long)cos_lut[k] * (long)y0;
    if (y1 >= 0) {
      x0 = x1 >> 15;
      y0 = y1 >> 15;
      phi += currentAngle;
    }
    currentAngle = currentAngle >> 1; //means: currAngle=currAngle/2;
    k++;
  }
  radius = x0;
  angle += ((sign) ? -phi : +phi); //faster than: if(sign) {angle-=phi;} else {angle+=phi;} ? To be confirmed...
  // angle 2pi = 256, size of char
}




/*
   10-bit CORDIC atan2sqrt
   Calculates the angle and radius of the vector pointing towards (x,y)
   The angle is returned as an unsigned int. 65536 means 2pi or 360 degrees. angle step resolution is 64.
   radius is the length of the vector as an int.
*/
void cordic10::atan2sqrt(int _x, int _y) {
  x0 = _x; y0 = _y; angle = 0; sign = 0;
  if (y0 < 0) {
    x0 = -x0;
    y0 = -y0;
    angle = 512;
  }
  if (x0 < 0) {
    tmp = x0;
    x0 = y0;
    y0 = -tmp;
    angle += 256;
  }
  if (y0 > x0) {
    sign = 1;
    angle += 256;
    tmp = x0;
    x0 = y0;
    y0 = tmp;
  }
  currentAngle = 64; phi = 0; k = 0;
  while (k <= 6) {
    x1 = (long)cos_lut[k] * (long)x0;
    x1 += (long)sin_lut[k] * (long)y0;
    y1 = (long)cos_lut[k] * (long)y0;
    y1 -= (long)sin_lut[k] * (long)x0;
    if (y1 >= 0) {
      x0 = x1 >> 15;
      y0 = y1 >> 15;
      phi += currentAngle;
    }
    currentAngle = currentAngle >> 1;
    k++;
  }
  radius = x0;
  angle += ((sign) ? -phi : +phi); //if(sign) {angle-=phi;} else {angle+=phi;}
  // angle 2pi = 1024
  angle = angle << 6; // stretch to the size of int. angle resolution is: 64
  // angle 2pi now is 65536
}




/*
   Source of 16-bit sincos adapted from: http://www.dcs.gla.ac.uk/~jhw/cordic/index.html

   Function is valid for arguments in range -pi/2 -- pi/2
   for values pi/2--pi: value = half_pi-(theta-half_pi) and similarly for values -pi---pi/2
   1.0 = 16384
   1/k = 0.6072529350088812561694
   pi = 3.1415926536897932384626

*/
void cordic16::sincos(int _theta, int _n) {
  x = CORDIC_1K;
  y = 0;
  z = _theta;
  n = (_n > CORDIC_NTAB) ? CORDIC_NTAB : _n;

  for (k = 0; k < n; ++k) {
    d = z >> 15; //get sign
    tx = x - (((y >> k) ^ d) - d);
    ty = y + (((x >> k) ^ d) - d);
    tz = z - ((cordic_ctab[k] ^ d) - d);
    x = tx; y = ty; z = tz;
  }
  cos = x; sin = y;
}



// end of cordic.cpp
