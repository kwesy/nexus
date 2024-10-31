
#include <receiver.h>
#include <Arduino.h>
#include <Config.h>


void Receiver::init() {

}

void Receiver::readChannels() 
{
  channels.roll = analogRead(CH1);
  channels.pitch = analogRead(CH2);
  channels.throttle = analogRead(CH3);
  channels.yaw = analogRead(CH4);
}

void Receiver::printChannels() 
{
    Serial.print("CH1 - ");
    Serial.println(analogRead(A0));
    Serial.print("CH2 - ");
    Serial.println(analogRead(A1));
    Serial.print("CH3 - ");
    Serial.println(analogRead(A2));
    Serial.print("CH4 - ");
    Serial.println(analogRead(A3));
}

