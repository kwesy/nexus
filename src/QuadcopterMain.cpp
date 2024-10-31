// // #include <Arduino.h>
// // #include <Wire.h>
// // #include "MPU6050.h"

// // MPU6050 MPU;

// // void setup()
// // {
// //   Serial.begin(9600);
// //   Wire.begin();
// //   MPU.init();
// //   MPU.setupMPU();
// //   // MPU.calibrate(5000);
// //   // MPU.printOffset();
// //   MPU.setOffset(-2.97, 0.41, 0.60, 0.06, -0.01, 0.05);
// // }

// // void loop()
// // {

// //   // MPU.recordAccelRegisters();
// //   // MPU.recordGyroRegisters();
// //   // MPU.recordTempRegisters();
// //   // MPU.printDataCSVFormat();
// //   // MPU.filter();
// //   // MPU.printTSP();
// //   MPU.Debuger2();
// //   // delay(300);
// // }
// // -------------------------------

// #include <Wire.h>
// #include <Arduino.h>
// #include <Quadcopter.h>

// Quadcopter qc;

// uint32_t LoopTimer;
// double dt = 0;
// double altRef = 0;
// double rollRef = 0;
// double pitchRef = 0;
// double yawRef = 0;

// uint32_t startTimer;



// void stablizeMode();
// void hover();
// void haltAfter(unsigned long);

// void setup() {
//   Serial.begin(57600);
//   qc.init();
//   delay(7000);
//   LoopTimer=micros();
//   startTimer = millis();

// }


// void loop() {
  
//   stablizeMode();

  
//   while (micros() - LoopTimer < 4000);
//   LoopTimer=micros();

//   haltAfter(25000);
// }







// void stablizeMode()
// {
//   qc.imu.recordSensorValues();
//   // qc.imu.printTSP();
//   dt = LoopTimer/ 1000000.0;
//   double pitch = qc.controllerPitch.computePID(pitchRef, qc.imu.getPitch(), dt);
//   double roll = qc.controllerRoll.computePID(rollRef, qc.imu.getRoll(), dt);
//   double yaw = qc.controllerYaw.computePID(yawRef, qc.imu.getYaw(), dt);
//   double thrust = qc.controllerThrust.computePID(altRef, qc.gps.getAltitude(), dt);


//   qc.motorMixer(thrust, roll, pitch, yaw);

//   // qc.motorMixer(70, roll, pitch, 0);
//   // delay(1000);
//   // qc.motorMixer(0, roll*(-1)*0, pitch*(-1)*0, -15);
//   // delay(1000);

//   // Serial.print(roll);
//   // Serial.print(" ");
//   // Serial.println(pitch);
// }


// void hover()
// {
//   qc.motorMixer(80, 0, 0, 0);
// }

// void getBatteryVoltage()
// {

// }

// void haltAfter(unsigned long t)
// {
//   while(millis() - startTimer > t){
//     qc.motorMixer(65, 0,0,0);
//       while(millis() - startTimer > t+3000){
//       qc.motorMixer(0,0,0,0);
//     }
//   }
// }

// // -----------------------


// // #include <Arduino.h>
// // #include <Servo.h>

// // Servo Motor1; 


// // void setup () {
// //   Serial.begin(9600);
// //   Motor1.attach(11,1000,2000);
// //   Motor1.write(0);
// // }

// // void loop(){

// //   int value = analogRead(4);
// //   value = map(value, 0, 1023, 0, 180);
// //   Motor1.write(value);
// //   Serial.println(value);
// // }