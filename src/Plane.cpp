#include <Plane.h>
#include <Config.h>

void Plane::init()
{
    Motor1.attach(PRT1,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor2.attach(PRT2,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor3.attach(PRT3,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor4.attach(PRT4,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

    Motor1.write(0);
    Motor2.write(0);
    Motor3.write(0);
    Motor4.write(0); 
}

void Plane::testMotors()
{
    Motor1.write(50);
    Motor2.write(50);
    Motor3.write(50);
    Motor4.write(50);

    delay(1000);

    Motor1.write(120);
    Motor2.write(120);
    Motor3.write(50);
    Motor4.write(120);

    delay(1000);

    Motor1.write(90);
    Motor2.write(90);
    Motor3.write(0);
    Motor4.write(90);
}

void Plane::setRoll(double angle)
{
    *refRoll = angle;
}

void Plane::setPitch(double angle)
{
    *refPitch = angle;
}

void Plane::setAltidute(double height)
{
    *refAltitude = height;
}

void Plane::motorMixer(float throttle, float roll, float pitch, float yaw) {
    Motor1.write(roll);  // aileron
    Motor2.write(pitch);   // elevator
    Motor3.write(throttle);   // thrust
    Motor4.write(yaw);   // rudder
}
