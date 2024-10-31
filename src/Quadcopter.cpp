#include <Quadcopter.h>


void Quadcopter::init()
{
    Motor1.attach(11,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor2.attach(10,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor3.attach(9,1000,2000); // (pin, min pulse width, max pulse width in microseconds) 
    Motor4.attach(3,1000,2000); // (pin, min pulse width, max pulse width in microseconds)

    Motor1.write(0);
    Motor2.write(0);
    Motor3.write(0);
    Motor4.write(0); 

    // Set pid tuning params
    // [0.01, 0.5, 0.005], [0.01, 2.0, 0.01], [0.0, 0.05, 0.01]
    controllerRoll.setGains(0.1, 0.0, 0.001, 0.15, 100, -100);
    controllerPitch.setGains(0.1, 0.0, 0.001, 0.15, 100, -100);
    // controllerPitch.setGains(0.6, 3.5, 0.03, 0.15, 20, -20);

    // Initiate the sensors
    imu.init();
    // imu.calibrate(2000);
    // imu.setOffset(-2.95, 0.46, 0.66, 0.05, -0.03, 0.07);
    imu.setOffset(-2.87, 0.47, 0.67, 0.04, -0.05, 0.07);
    imu.printOffset();
}

void Quadcopter::testMotors()
{
    Motor1.write(0);
    Motor2.write(0);
    Motor3.write(0);
    Motor4.write(0);

    delay(1000);

    Motor1.write(50);
    Motor2.write(50);
    Motor3.write(50);
    Motor4.write(50);

    delay(1000);

    Motor1.write(0);
    Motor2.write(0);
    Motor3.write(0);
    Motor4.write(0);
}

void Quadcopter::setRoll(double angle)
{
    *refRoll = angle;
}

void Quadcopter::setPitch(double angle)
{
    *refPitch = angle;
}

void Quadcopter::setAltidute(double height)
{
    *refAltitude = height;
}

void Quadcopter::motorMixer(float throttle, float roll, float pitch, float yaw) {
    Motor1.write(throttle + roll - pitch + yaw);  // front-left
    Motor2.write(throttle - roll - pitch - yaw);   // front-right
    Motor3.write(throttle + roll + pitch - yaw);   // rear-left
    Motor4.write(throttle - roll + pitch + yaw);   // rear-right

    // Serial.print(throttle + roll - pitch - yaw);  // front-left
    // Serial.print(" ");
    // Serial.print(throttle - roll - pitch + yaw);   // front-right
    // Serial.print(" ");
    // Serial.print(throttle - roll + pitch - yaw);   // rear-left
    // Serial.print(" ");
    // Serial.println(throttle + roll + pitch + yaw);   // rear-right
}





