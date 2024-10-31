
#ifndef PLANE_H
#define PLANE_H

#include <Arduino.h>
#include <Servo.h>
// #include <GPS.h>


class Plane
{

private:
    float *refRoll = 0, *refPitch = 0, *refAltitude = 0;
    Servo Motor1;     // create servo object to control the aileron servo
    Servo Motor2;     // create servo object to control the elevator servo
    Servo Motor3;     // create servo object to control the ESC 1
    Servo Motor4;     // create servo object to control the rudder servo

public:

    //Sensors
    // GPS gps;



    void init();
    void testMotors();
    void setRoll(double angle);
    void setPitch(double angle);
    void setAltidute(double altitude);
    void motorMixer(float throttle, float roll, float pitch, float yaw);


};

#endif

