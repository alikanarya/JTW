#ifndef PLCCHECKTHREAD_H
#define PLCCHECKTHREAD_H

#include <QThread>

#include "../_Modules/S7/s7.h"

class plcCheckThread : public QThread{
    Q_OBJECT

public:
    s7 *plc;                                // pointer to plc class object
    int result;
    bool plcInteract;

    plcCheckThread(s7 &_plc);
    void stop();
    //~plcCheckThread();

protected:
    void run();

private:
    volatile bool stopped;

};

#endif // PLCCHECKTHREAD_H
