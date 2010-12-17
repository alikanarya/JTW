#include "getsystemtime.h"

getSystemTime::getSystemTime(){}

int getSystemTime::getSystemTimeMsec(){

    LARGE_INTEGER ticksPerSecond;
    LARGE_INTEGER tick;   // A point in time
    LARGE_INTEGER time;   // For converting tick into real time

    // get the high resolution counter's accuracy
    QueryPerformanceFrequency(&ticksPerSecond);

    // what time is it?
    QueryPerformanceCounter(&tick);

    // convert the tick number into the number of seconds
    // since the system was started...
    time.QuadPart = tick.QuadPart/ticksPerSecond.QuadPart;
    hour = time.QuadPart / 3600;
    time.QuadPart = time.QuadPart - (hour * 3600);
    minute = time.QuadPart / 60;
    second = time.QuadPart - (minute * 60);
    msec = ((tick.QuadPart % ticksPerSecond.QuadPart)*1000/ticksPerSecond.QuadPart);

    return ( (hour * 3600 + minute * 60 + second) * 1000 + msec );
}

getSystemTime::~getSystemTime(){}
