#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>

#include "getsystemtime.h"

class playThread : public QThread {
    Q_OBJECT

    public:
        getSystemTime time;                                             // system time class

        playThread();                                                   // constructor
        void run();                                                     // run thread
        int calcTotalMsec(int hour, int min, int second, int msec);     // calc. total msec of time values

};

#endif // PLAYTHREAD_H
