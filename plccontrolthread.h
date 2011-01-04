#ifndef PLCCONTROLTHREAD_H
#define PLCCONTROLTHREAD_H

#include <QThread>

#include "../_Modules/S7/s7.h"

class plcControlThread : public QThread{
    Q_OBJECT

public:
    s7 *plc;                                // pointer to plc class
    int plcType;
    QString plcUrl;
    int dbNo;
    int right_BYTE, right_BIT;
    int left_BYTE, left_BIT;
    int stop_BYTE, stop_BIT;
    int emergency_BYTE, emergency_BIT;

    int commandState;
    bool result;
    int checkResult;
    bool plcInteract;


    plcControlThread(int type, QString url);
    plcControlThread(s7 &_plc);
    void stop();
    bool plcCmdCenter();
    bool plcCmdRight();
    bool plcCmdLeft();
    bool plcCmdStop();
    bool plcCmdStopReset();
    bool plcCmdEmergencyAct();
    bool plcCmdEmergencyPsv();
    void check();
    void disconnect();
    ~plcControlThread();

protected:
    void run();

private:
    volatile bool stopped;

};

#endif // PLCCONTROLTHREAD_H
