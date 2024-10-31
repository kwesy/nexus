#include <PlaneMain.h>

Plane plane;
Receiver rx;

void runPlane()
{
    rx.readChannels();
    plane.motorMixer(rx.channels.throttle, rx.channels.roll, rx.channels.pitch, rx.channels.yaw);
    rx.printChannels();
    delay(2000);
}

