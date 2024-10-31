#ifndef MPU6050_H
#define MPU6050_H

#include <Arduino.h>

class MPU6050

{

public:

    int16_t AccXLSB, AccYLSB, AccZLSB;
    float accelX, accelY, accelZ;

    int16_t GyroX, GyroY, GyroZ;
    float gyroRateX, gyroRateY, gyroRateZ;

    float aAngleX, aAngleY;  // unfiltered angles
    float roll=0, pitch=0, yaw=0; // estimated angles

    float KalmanUncertaintyAngleRoll=2*2;
    float KalmanUncertaintyAnglePitch=2*2;

    long temp;
    float tmp;

    float accelXOffset = 0;
    float accelYOffset = 0;
    float accelZOffset = 0;
    float gyroXOffset = 0;
    float gyroYOffset = 0;
    float gyroZOffset = 0;

    void init();
    void recordAccelRegisters();
    void recordGyroRegisters();
    void recordTempRegisters();
    void processGyroData();
    void processAccelData();
    // void processTempData();
    void calibrate(int num_samples);
    void filter();
    void recordSensorValues();
    void Debuger();
    void Debuger2();
    void printDataCSVFormat();
    void printOffset();
    void setOffset(float gyroOffsetX, float gyroOffsetY, float gyroOffsetZ, float accelOffsetX, float accelOffsetY, float accelOffsetZ);
    float getRoll();
    float getPitch();
    float getYaw();
    void printTSP();
    void calcAltitude();
};

#endif // MPU6050_H
