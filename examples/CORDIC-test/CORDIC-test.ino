/*
 * File: CORDIC-test.ino
 * Purpose: Test program for evaluating the CORDIC atan2sqrt function
 * Version: 1.0.0
 * Date: 12-07-2019
 * URL: https://github.com/MartinStokroos/fastCORDIC
 * License: MIT License
*/

#include <cordic.h>

#define LED_PIN 13 // debug output pin

unsigned long startTime;
unsigned long execTime;
long x, y;
double angle;
int k;

cordic8 cordic;    //test cordic8, the 8-bit implementation
//cordic10 cordic;  //test cordic10, the 10-bit implementation (but slower...)


void setup()
  {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);  // when using Serial.print() in the function loop()
  Serial.println();
  Serial.println();

  Serial.print("sin-lut: ");
  for (k=0; k<=6; k++){
    Serial.print(sin_lut[k]);
      Serial.print(", ");
        }
  Serial.println();

  Serial.print("cos-lut: ");
  for (k=0; k<=6; k++){
    Serial.print(cos_lut[k]);
      Serial.print(", ");
        }
  Serial.println();

  Serial.print("tan-lut: ");
  for (k=0; k<=5; k++){
    Serial.print(tan_lut[k]);
      Serial.print(", ");
        }
  Serial.println();
  Serial.println();
  }



void loop(){
  // get random input
  x = random(-1000, 1000);
  y = random(-1000, 1000);

  // test atan2sqrt
  startTime = micros();
  cordic.atan2sqrt(x, y);
  execTime = micros()-startTime;

  Serial.print("x=");
  Serial.print(x);
  Serial.print(", ");
  Serial.print("y=");
  Serial.println(y);

  Serial.print("CORDIC: ");
  Serial.print("angle=");
  Serial.print((float)cordic.angle*360/256, 1);   //test cordic8
  //Serial.print((float)cordic.angle*360/65536, 1); //test cordic10
  Serial.print(", ");
  Serial.print("radius=");
  Serial.print((float)cordic.radius, 1);  //test cordic8
  //Serial.print((float)cordic.radius*1000/1024, 1);  //test cordic10
  Serial.print(", ");
  Serial.print("exec.time(us): ");
  Serial.println(execTime);

  // test atan2
  startTime = micros();
  angle = atan2(y,x);
  execTime = micros()-startTime;

  angle = 180*angle/PI;
  if (angle<0){
    angle = 360+angle;
  }
  Serial.print("ATAN2: ");
  Serial.print("angle=");
  Serial.print(angle,1);
  Serial.print(", ");
  Serial.print("exec.time(us): ");
  Serial.println(execTime);
  Serial.println();

  Serial.print("Press any key to repeat... ");
  while(Serial.available() == 0){;}
  Serial.read();
  Serial.println();
}

