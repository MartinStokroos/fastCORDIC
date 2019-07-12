# fastCORDIC
Fast CORDIC algorithm for ATAN2 on the Arduino.

This library is a collection (under construction) of trigonometric functions, efficiently
calculated with the COordinate Rotation DIgital Computer (CORDIC) algorithm.
The function implemented here is the atan2 function, which calculates the angle and
radius (length) of the vector that points to (x,y). This function is very useful in DSP
applications such as digital down conversion.

The CORDIC implementation of ATAN2 is up to twice as fast as the ATAN2 from the standard library.
The CORDIC algorithm rotates a vector until it is aligned with the positive x-axis.
In this implementation the length of the vector is retained and thus, the square-root
of x^2+y^2 is known and equals to the value of x after rotation.

An example sketch is provided for benchmarking the CORDIC routine.

Maybe someone could help me optimizing this code for ultimately the fastest solution possible on the Arduino.... 
