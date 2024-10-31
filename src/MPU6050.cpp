#include <Wire.h>
#include "MPU6050.h"
#include <Arduino.h>


float Kalman1DOutput[]={0,0};


void MPU6050::init()
{
    Wire.setClock(400000);
    Wire.begin();
    delay(250);

    Wire.beginTransmission(0b1101000); // This is the I2C address of the MPU
    Wire.write(0x6B);                  // Accessing the register 6B - Power Management
    Wire.write(0b00000000);            // Setting SLEEP register to 0
    Wire.endTransmission();

    // Wire.beginTransmission(0b1101000); // I2C address of the MPU
    // Wire.write(0x1B);                  // Accessing the register 1B - Gyroscope Configuration
    // Wire.write(0x00000000);            // Setting the gyro to full scale +/- 250deg./s
    // Wire.endTransmission();

    // Wire.beginTransmission(0b1101000); // I2C address of the MPU
    // Wire.write(0x1C);                  // Accessing the register 1C - Accelerometer Configuration
    // Wire.write(0b00000000);            // Setting the accel to +/- 2g
    // Wire.endTransmission();
}


void MPU6050::recordAccelRegisters()
{
    Wire.beginTransmission(0x68);
    Wire.write(0x1A);
    Wire.write(0x05);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x1C);
    Wire.write(0x10);
    Wire.endTransmission();
    Wire.beginTransmission(0x68);
    Wire.write(0x3B);
    Wire.endTransmission(); 
    Wire.requestFrom(0x68,6);
    AccXLSB = Wire.read() << 8 | Wire.read();
    AccYLSB = Wire.read() << 8 | Wire.read();
    AccZLSB = Wire.read() << 8 | Wire.read();
}

void MPU6050::recordTempRegisters()
{
    Wire.beginTransmission(0b1101000);
    Wire.write(0x41); // Starting register for temperature Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000, 2); // Request temperature Registers (41 - 42)
    while (Wire.available() < 2)
        ;
    temp = Wire.read() << 8 | Wire.read();
    tmp = temp / 340.00 + 36.53;
}

void MPU6050::recordGyroRegisters()
{
    Wire.beginTransmission(0x68);
    Wire.write(0x1B); 
    Wire.write(0x8);
    Wire.endTransmission();     
    Wire.beginTransmission(0x68);
    Wire.write(0x43);
    Wire.endTransmission();
    Wire.requestFrom(0x68,6);
    GyroX=Wire.read()<<8 | Wire.read();
    GyroY=Wire.read()<<8 | Wire.read();
    GyroZ=Wire.read()<<8 | Wire.read();
}

void MPU6050::processAccelData()
{
    accelX=(float)AccXLSB/4096 - accelXOffset;
    accelY=(float)AccYLSB/4096 - accelYOffset;
    accelZ=(float)AccZLSB/4096 - accelZOffset;

    aAngleX=atan(accelY/sqrt(accelX*accelX+accelZ*accelZ))*1/(3.142/180);
    aAngleY=-atan(accelX/sqrt(accelY*accelY+accelZ*accelZ))*1/(3.142/180);
}

void MPU6050::processGyroData()
{
    gyroRateX = (GyroX / 65.5f) - gyroXOffset;
    gyroRateY = (GyroY / 65.5f) - gyroYOffset;
    gyroRateZ = (GyroZ / 65.5f) - gyroZOffset;
}

void MPU6050::calibrate(int num_samples)
{
    for (int i = 0; i < 1000; i++) //throw away first 1000 readings
    {
        recordAccelRegisters();
        recordGyroRegisters();
    }


    float gyroXSum = 0, gyroYSum = 0, gyroZSum = 0, accelXSum = 0, accelYSum = 0, accelZSum = 0;
    for (int i = 0; i < num_samples; i++)
    {
        recordAccelRegisters();
        processAccelData();
        recordGyroRegisters();
        processGyroData();
        filter(); //Strangely this help with offsetting roll and pitch to zero 

        gyroXSum += gyroRateX;
        gyroYSum += gyroRateY;
        gyroZSum += gyroRateZ;
        accelXSum += accelX;
        accelYSum += accelY;
        accelZSum += accelZ;

        delay(2); // Small delay to allow for better averaging
    }

    gyroXOffset = gyroXSum / num_samples;
    gyroYOffset = gyroYSum / num_samples;
    gyroZOffset = gyroZSum / num_samples;
    accelXOffset = accelXSum / num_samples;
    accelYOffset = accelYSum / num_samples;
    accelZOffset = accelZSum / num_samples - 1.0; // 1g correction for Z axis

    // Reset after calibration
    aAngleX = 0.0f;
    aAngleY = 0.0f;
}

float complementaryFilter(float angle, float gyroRate, float accelAngle, float alpha)
{
    return alpha * (angle + gyroRate) + (1 - alpha) * accelAngle;
}

void kalman_1d(float KalmanState, float KalmanUncertainty, float KalmanInput, float KalmanMeasurement) {
  KalmanState=KalmanState+0.004*KalmanInput;
  KalmanUncertainty=KalmanUncertainty + 0.004 * 0.004 * 4 * 4;
  float KalmanGain=KalmanUncertainty * 1/(1*KalmanUncertainty + 3 * 3);
  KalmanState=KalmanState+KalmanGain * (KalmanMeasurement-KalmanState);
  KalmanUncertainty=(1-KalmanGain) * KalmanUncertainty;
  Kalman1DOutput[0]=KalmanState; 
  Kalman1DOutput[1]=KalmanUncertainty;
}

void MPU6050::filter()
{
  kalman_1d(roll, KalmanUncertaintyAngleRoll, gyroRateX, aAngleX);
  roll=Kalman1DOutput[0]; 
  KalmanUncertaintyAngleRoll=Kalman1DOutput[1];
  kalman_1d(pitch, KalmanUncertaintyAnglePitch, gyroRateY, aAngleY);
  pitch=Kalman1DOutput[0]; 
  KalmanUncertaintyAnglePitch=Kalman1DOutput[1];
}

void MPU6050::setOffset(float gyroOffsetX, float gyroOffsetY, float gyroOffsetZ, float accelOffsetX, float accelOffsetY, float accelOffsetZ)
{
    gyroXOffset = gyroOffsetX;
    gyroYOffset = gyroOffsetY;
    gyroZOffset = gyroOffsetZ;
    accelXOffset = accelOffsetX;
    accelYOffset = accelOffsetY;
    accelZOffset = accelOffsetZ;
}

void MPU6050::recordSensorValues()
{
    recordGyroRegisters();
    recordAccelRegisters();
    processGyroData();
    processAccelData();
    filter();
}

float MPU6050::getRoll()
{
    return roll;
}

float MPU6050::getPitch()
{
    return pitch;
}

void MPU6050::Debuger()
{
    Serial.print("Gyro (rate)");
    Serial.print(" X=");
    Serial.print(gyroRateX);
    Serial.print(" Y=");
    Serial.print(gyroRateY);
    Serial.print(" Z=");
    Serial.print(gyroRateZ);
    Serial.print(" Accel (g)");
    Serial.print(" X=");
    Serial.print(accelX);
    Serial.print(" Y=");
    Serial.print(accelY);
    Serial.print(" Z=");
    Serial.print(accelZ);
    Serial.print(" Temp=");
    Serial.print(tmp);
    delay(1000);
}


void MPU6050::Debuger2()
{
    MPU6050::recordAccelRegisters();
    MPU6050::recordGyroRegisters();
    // MPU6050::recordTempRegisters();
    MPU6050::filter();
    Serial.print("[DEBUG MODE] ");
    Serial.print("Gyro (rate)");
    Serial.print(" X=");
    Serial.print(gyroRateX);
    Serial.print(" Accel");
    Serial.print(" X=");
    Serial.print(accelX);
    // Serial.print(" Temp=");
    // Serial.print(tmp);
    Serial.print(" A_Angle");
    Serial.print(" X=");
    Serial.print(aAngleX);
    Serial.print(" F_Angle");
    Serial.print(" X=");
    Serial.print(roll);
    Serial.println();
}

void MPU6050::printDataCSVFormat()
{

    // Serial.print("A");
    Serial.print(aAngleX);
    Serial.print(" ");
    // Serial.print(aAngleY);
    // Serial.print(" ");
    // Serial.print(filteredAngleX);
    // Serial.print(" ");
    // Serial.print(filteredAngleY);
    // Serial.print(" ");
    // Serial.print(filteredAngleZ);
    // Serial.print(" ");
    // Serial.println(tmp);
    // Serial.println();
}

void MPU6050::printOffset()
{
    Serial.print(gyroXOffset);
    Serial.print(" ");
    Serial.print(gyroYOffset);
    Serial.print(" ");
    Serial.print(gyroZOffset);
    Serial.print(" ");
    Serial.print(accelXOffset);
    Serial.print(" ");
    Serial.print(accelYOffset);
    Serial.print(" ");
    Serial.print(accelZOffset);
    Serial.println();
}

void MPU6050::printTSP()
{
    Serial.print("Roll [°]");
    Serial.print(roll);
    Serial.print(" Pitch [°]");
    Serial.print(pitch);
    // Serial.print("Z");
    // Serial.print(filteredAngleZ);
    Serial.println();
}

void MPU6050::calcAltitude()
{
    
}