#include <Wire.h>
#include "MPU6050.h"
#include <Arduino.h>

void MPU6050::init()
{
    Wire.setClock(400000);
    Wire.begin();
    delay(250);

    Wire.beginTransmission(0b1101000); // This is the I2C address of the MPU
    Wire.write(0x6B);                  // Accessing the register 6B - Power Management
    Wire.write(0b00000000);            // Setting SLEEP register to 0
    Wire.endTransmission();

    Wire.beginTransmission(0b1101000); // I2C address of the MPU
    Wire.write(0x1B);                  // Accessing the register 1B - Gyroscope Configuration
    Wire.write(0x00000000);            // Setting the gyro to full scale +/- 250deg./s
    Wire.endTransmission();

    Wire.beginTransmission(0b1101000); // I2C address of the MPU
    Wire.write(0x1C);                  // Accessing the register 1C - Accelerometer Configuration
    Wire.write(0b00000000);            // Setting the accel to +/- 2g
    Wire.endTransmission();
}


void MPU6050::recordAccelRegisters()
{
    Wire.beginTransmission(0b1101000); // I2C address of the MPU
    Wire.write(0x3B);                  // Starting register for Accel Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000, 6); // Request Accel Registers (3B - 40)
    while (Wire.available() < 6)
        ;
    accelX = Wire.read() << 8 | Wire.read(); // Store first two bytes into accelX
    accelY = Wire.read() << 8 | Wire.read(); // Store middle two bytes into accelY
    accelZ = Wire.read() << 8 | Wire.read(); // Store last two bytes into accelZ
    MPU6050::processAccelData();
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
    Wire.beginTransmission(0b1101000); // I2C address of the MPU
    Wire.write(0x43);                  // Starting register for Gyro Readings
    Wire.endTransmission();
    Wire.requestFrom(0b1101000, 6); // Request Gyro Registers (43 - 48)
    while (Wire.available() < 6)
        ;
    gyroX = Wire.read() << 8 | Wire.read(); // Store first two bytes into gyroX
    gyroY = Wire.read() << 8 | Wire.read(); // Store middle two bytes into gyroY
    gyroZ = Wire.read() << 8 | Wire.read(); // Store last two bytes into gyroZ

    MPU6050::processGyroData();
}

void MPU6050::processAccelData()
{
    accelX = (accelX / 16380.0f) - accelXOffset;
    accelY = (accelY / 16380.0f) - accelYOffset;
    accelZ = (accelZ / 16380.0f) - accelZOffset;

    aAngleX = atan2(accelY , accelZ ) * 180 / PI;
    // aAngleY = atan2(accelX / 9.8, accelZ / 9.8) * 180 / PI;
    // aAngleX = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180 / PI;
    aAngleY = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;
}

void MPU6050::processGyroData()
{
    gyroX = (gyroX / 131.0f) - gyroXOffset;
    gyroY = (gyroY / 131.0f) - gyroYOffset;
    gyroZ = (gyroZ / 131.0f) - gyroZOffset;

    curTime = millis();
    dt = (curTime - prevTime) / 1000.0f;

    gAngleX += gyroX * dt;
    gAngleY += gyroY * dt;
    gAngleZ += gyroZ * dt;

    prevTime = curTime;
}

void MPU6050::calibrate(int num_samples)
{
    float gyroXSum = 0, gyroYSum = 0, gyroZSum = 0, accelXSum = 0, accelYSum = 0, accelZSum = 0;
    for (int i = 0; i < num_samples; i++)
    {
        MPU6050::recordAccelRegisters();
        MPU6050::recordGyroRegisters();

        gyroXSum += gyroX;
        gyroYSum += gyroY;
        gyroZSum += gyroZ;
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
    gAngleX = 0.0f;
    gAngleY = 0.0f;
    gAngleZ = 0.0f;
    aAngleX = 0.0f;
    aAngleY = 0.0f;
}

float complementaryFilter(float angle, float gyroRate, float accelAngle, float alpha)
{
    return alpha * (angle + gyroRate) + (1 - alpha) * accelAngle;
}

void MPU6050::filter()
{
    filteredAngleX = complementaryFilter(filteredAngleX, gAngleX, aAngleX, 0.995);
    filteredAngleY = complementaryFilter(filteredAngleY, gAngleY, aAngleY, 0.95);
    filteredAngleZ = complementaryFilter(filteredAngleZ, gAngleZ, 0, 0.95); // no accelAngleZ
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

void MPU6050::Debuger()
{
    Serial.print("Gyro (rate)");
    Serial.print(" X=");
    Serial.print(gyroX);
    Serial.print(" Y=");
    Serial.print(gyroY);
    Serial.print(" Z=");
    Serial.print(gyroZ);
    Serial.print(" Accel (g)");
    Serial.print(" X=");
    Serial.print(accelX);
    Serial.print(" Y=");
    Serial.print(accelY);
    Serial.print(" Z=");
    Serial.print(accelZ);
    Serial.print(" Temp=");
    Serial.print(tmp);
    Serial.print(" dt=");
    Serial.println(dt);
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
    Serial.print(gyroX);
    Serial.print(" Accel");
    Serial.print(" X=");
    Serial.print(accelX);
    // Serial.print(" Temp=");
    // Serial.print(tmp);
    Serial.print(" G_Angle");
    Serial.print(" X=");
    Serial.print(gAngleX);
    Serial.print(" A_Angle");
    Serial.print(" X=");
    Serial.print(aAngleX);
    Serial.print(" F_Angle");
    Serial.print(" X=");
    Serial.print(filteredAngleX);
    Serial.print(" dt=");
    Serial.println(dt);
}

void MPU6050::printDataCSVFormat()
{

    // Serial.print("G");
    Serial.print(gAngleX);
    Serial.print(" ");
    // Serial.print(gAngleY);
    // Serial.print(" ");
    // Serial.print(gAngleZ);
    // Serial.print(" ");
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
    // Serial.print("X");
    Serial.print(filteredAngleX);
    // Serial.print("Y");
    // Serial.print(filteredAngleY);
    // Serial.print("Z");
    // Serial.print(filteredAngleZ);
    Serial.println();
}
