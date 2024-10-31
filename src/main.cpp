// #include <Arduino.h>
// #include <Wire.h>
// #include "MPU6050.h"

// MPU6050 MPU;

// void setup()
// {
//   Serial.begin(9600);
//   Wire.begin();
//   MPU.init();
//   MPU.setupMPU();
//   // MPU.calibrate(5000);
//   // MPU.printOffset();
//   MPU.setOffset(-2.97, 0.41, 0.60, 0.06, -0.01, 0.05);
// }

// void loop()
// {

//   // MPU.recordAccelRegisters();
//   // MPU.recordGyroRegisters();
//   // MPU.recordTempRegisters();
//   // MPU.printDataCSVFormat();
//   // MPU.filter();
//   // MPU.printTSP();
//   MPU.Debuger2();
//   // delay(300);
// }
// -------------------------------
// #include <Arduino.h>
// #include <PlaneMain.h>

// uint32_t LoopTimer;
// double dt = 0;

// uint32_t startTimer;

// void haltAfter(unsigned long);

// void setup() {
//   Serial.begin(9600);
//   plane.init();
//   LoopTimer=micros();
//   startTimer = millis();
// }


// void loop() {
//   runPlane();
//   while (micros() - LoopTimer < 4000);
//   LoopTimer=micros();

// }

// void haltAfter(unsigned long t)
// {
//   while(millis() - startTimer > t){
//   }
// }





#include <Arduino.h>
#include <Servo.h>

Servo Motor1; 


void setup () {
  // pinMode(13, OUTPUT);
  Serial.begin(9600);
  Motor1.attach(3,1000,2000);;
}

void loop(){
  int potValue = analogRead(A0);
  int mappedValue = map(potValue, 0, 1023, 0, 180);
  Serial.println(mappedValue);
  delay(500);
  Motor1.write(mappedValue);
}


