# fastCORDIC
Fast CORDIC algorithm for ATAN2 on the Arduino.

This library is a collection (under construction) of trigonometric functions, efficiently
calculated with the COordinate Rotation DIgital Computer (CORDIC) algorithm.



The function currently implemented are:

- *atan2sqrt* as 8-bit and 10-bit CORDIC

  calculates the angle and the Hypotenuse (length of the vector) that points to (x,y). This function is very useful in DSP
  applications such as digital down conversion.

- *sincos* as 16-bit (int) CORDIC 

The CORDIC implementation of ATAN2 is up to twice as fast as the *atan2()* function from the standard 
Arduino library. 

In *atan2sqrt*,  CORDIC rotates the input vector clockwise until it is aligned with the positive
x-axis. In this implementation the length of the vector is retained and thus, the square-root
of x^2^ + y^2^ is known and equals to the value of x after rotation and rescaling.

An example sketch is provided to benchmark the different CORDIC routine(s).

