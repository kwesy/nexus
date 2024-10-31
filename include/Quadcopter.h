
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <Servo.h>
#include <simplePID.h>
#include <MPU6050.h>
#include <GPS.h>


class Quadcopter
{

private:
    float *refRoll = 0, *refPitch = 0, *refAltitude = 0;
    Servo Motor1;     // create servo object to control the ESC 1
    Servo Motor2;     // create servo object to control the ESC 2
    Servo Motor3;     // create servo object to control the ESC 3
    Servo Motor4;     // create servo object to control the ESC 4

public:

    //PID controllers
    SimplePID controllerRoll;
    SimplePID controllerPitch;
    SimplePID controllerYaw;
    SimplePID controllerThrust;

    //Sensors
    MPU6050 imu;
    GPS gps;



    void init();
    void testMotors();
    void setRoll(double angle);
    void setPitch(double angle);
    void setAltidute(double altitude);
    void motorMixer(float throttle, float roll, float pitch, float yaw);


    // void hover(double altitude);
    // void getAltitude();
};

#endif

