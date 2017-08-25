#ifndef PLCCONTROLTHREAD_H
#define PLCCONTROLTHREAD_H

#define MASK_CMD_CENTER 0   // 0000
#define MASK_CMD_RIGHT  1   // 0001
#define MASK_CMD_LEFT   2   // 0010
#define MASK_CMD_STOP   4   // 0100
#define MASK_CMD_EMG    12  // 1100

#define MASK_CMD_Z_CENTER   0   // 0000
#define MASK_CMD_Z_UP       1   // 0001
#define MASK_CMD_Z_DOWN     2   // 0010

#include <QThread>

#include "../_Modules/S7/s7.h"

class plcControlThread : public QThread{
    Q_OBJECT

public:
    s7 *plc;                                // pointer to plc class
    int plcType;
    QString plcUrl;
    int dbNo;
    int dbNoRead;
    int readByte = 0;
    int byteNo;
    int byteNo2;
    /*
    int right_BYTE, right_BIT;
    int left_BYTE, left_BIT;
    int stop_BYTE, stop_BIT;
    int emergency_BYTE, emergency_BIT;
    */

    int commandState;
    int commandZState;
    bool commandRead;
    bool result;
    int checkResult;
    bool plcInteract;

    int readLength;
    unsigned char* readBuffer;
    unsigned char* readBufferInt;
    int distanceRaw;


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

    bool plcCmd_Z_Center();
    bool plcCmd_Z_Up();
    bool plcCmd_Z_Down();

    bool readPLC(int byte);
    bool readDistanceValue(int byte, int length);

    int getBitofByte(unsigned char byte, int bitNo);

    void check();
    void disconnect();
    ~plcControlThread();

protected:
    void run();

private:
    volatile bool stopped;

private slots:
    void terminateThis();

};

#endif // PLCCONTROLTHREAD_H
