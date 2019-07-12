/*
 *
 * File: cordic.h
 * Purpose: CORDIC based trigonometric functions library for Arduino
 * Version: 1.0.0
 * Release Date: 28-06-2019
 *
 *
 * URL: https://github.com/MartinStokroos/fastCORDIC
 *
 * MIT License
 *
 * Copyright (c) 2019 Martin Stokroos
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

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
*/

#ifndef cordic_H_
#define cordic_H_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	sin, cos, tan table calculations, scaled to 32768
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	2pi = 256
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sin table for phi=16,8,4,2 and 1
 * 32768*sin((16/256)*360) = 12540
 * 32768*sin((8/256)*360) = 6393
 * 32768*sin((4/256)*360) = 3212
 * 32768*sin((2/256)*360) = 1608
 * 32768*sin((1/256)*360) = 804
 *
 * cos table for phi=16,8,4,2 and 1
 * 32768*cos((16/256)*360) = 30274
 * 32768*cos((8/256)*360) = 32138
 * 32768*cos((4/256)*360) = 32610
 * 32768*cos((2/256)*360) = 32729
 * 32768*cos((1/256)*360) = 32758
 *
 * tan table for phi=16,8,4,2 and 1
 * 32768*tan((16/256)*360) = 13573
 * 32768*tan((8/256)*360) = 6518
 * 32768*tan((4/256)*360) = 3227
 * 32768*tan((2/256)*360) = 1610
 * 32768*tan((1/256)*360) = 804
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	2pi = 1024
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sin table for phi=64,32,16,8,4,2 and 1
 * 32768*sin((64/1024)*360) = 12540
 * 32768*sin((32/1024)*360) = 6393
 * 32768*sin((16/1024)*360) = 3212
 * 32768*sin((8/1024)*360) = 1608
 * 32768*sin((4/1024)*360) = 804
 * 32768*sin((2/1024)*360) = 402
 * 32768*sin((1/1024)*360) = 201
 *
 * cos table for phi=64,32,16,8,4,2 and 1
 * 32768*cos((64/1024)*360) = 30274
 * 32768*cos((32/1024)*360) = 32138
 * 32768*cos((16/1024)*360) = 32610
 * 32768*cos((8/1024)*360) = 32729
 * 32768*cos((4/1024)*360) = 32758
 * 32768*cos((2/1024)*360) = 32766
 * 32768*cos((1/1024)*360) = 32767
 *
 * tan table for phi=64,32,16,8,4,2 and 1
 * 32768*tan((32/1024)*360) = 6518
 * 32768*tan((16/1024)*360) = 3227
 * 32768*tan((8/1024)*360) = 1610
 * 32768*tan((4/1024)*360) = 804
 * 32768*tan((2/1024)*360) = 402
 * 32768*tan((1/1024)*360) = 201
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


/*
* Creating look-up tables. There is overlap between 8-bit and 10-bit tables.
* 8-bit: phi={64,32,16,8,4,2,1}, address with index range 0-4
* 10-bit: phi={64,32,16,8,4,2,1}, address with index range 0-6 (0-5 for tan_lut)
*/ 
const unsigned int sin_lut[7] = {12540, 6393, 3212, 1608, 804, 402, 201};
const unsigned int cos_lut[7] = {30274, 32138, 32610, 32729, 32758, 32766, 32767};
const unsigned int tan_lut[6] = {32768, 13573, 6518, 3227, 1610, 804};


class cordic8
{
public:
	void atan2sqrt(int _x, int _y);
	unsigned char angle;
	unsigned int radius;

private:
	int x0, y0, tmp;
	long x1, y1;
	unsigned char phi, currentAngle, k;
	bool sign;
};



class cordic10
{
public:
	void atan2sqrt(int _x, int _y);
	unsigned int angle;
	unsigned int radius;

private:
	int x0, y0, tmp;
	long x1, y1;
	unsigned char k;
	unsigned int phi, currentAngle;
	bool sign;
};

#endif /* cordic_H_ */
