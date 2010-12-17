#ifndef GETSYSTEMTIME_H
#define GETSYSTEMTIME_H

#include "windows.h"

class getSystemTime{

    public:
        int hour;
        int minute;
        int second;
        int msec;

        getSystemTime();            // constructor
        int getSystemTimeMsec();    // returns elapsed time in msec since last boot of the system
        ~getSystemTime();           // destructor

};

#endif // GETSYSTEMTIME_H
