#ifndef PLCCONTROLTHREAD_H
#define PLCCONTROLTHREAD_H

#include <QThread>

#include "s7.h"

class plcControlThread : public QThread{
    Q_OBJECT

public:
    s7 *plc;                                // pointer to plc class
    int commandState;
    int result;
    bool plcInteract;

    plcControlThread(s7 &_plc);
    void stop();
    int plcCmdCenter();
    int plcCmdRight();
    int plcCmdLeft();
    int plcCmdStop();
    int plcCmdStopReset();
    int plcCmdEmergencyAct();
    int plcCmdEmergencyPsv();
    void check();
//    void run();

protected:
    void run();

private:
    volatile bool stopped;

};

#endif // PLCCONTROLTHREAD_H
