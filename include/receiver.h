#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver
{  
public:
    struct
    {
        int throttle = 0;
        int yaw = 0;
        int pitch = 0;
        int roll = 0;

    } channels;
    
    void init();
    void readChannels();
    void printChannels();

};


#endif