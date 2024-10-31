#ifndef GPS_H
#define GPS_H

//Include GPS helper libraries headers here


class GPS
{
private:
    /* data */
    double alt;

public:
    GPS(/* args */);
    ~GPS();

    double getAltitude();
};


#endif