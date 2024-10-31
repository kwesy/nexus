
#ifndef TRANCEIVER_H
#define TRANCEIVER_H

class Transmitter
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
    void sendSignal(int *text);
    

};


#endif