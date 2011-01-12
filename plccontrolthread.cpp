#include "plccontrolthread.h"

#include "mainwindow.h"

plcControlThread::plcControlThread(int type, QString url){
    plcType = type;
    plcUrl = url;
    plc = new s7();

    plc->changeType(plcType);
    plcType = plc->plcType;  // to fix in case of wrong selection

    if (plcType == 0) dbNo = 1;    // for S7-200

    stopped = false;
    result = false;
}

plcControlThread::plcControlThread(s7 &_plc){
    stopped = false;
    result = false;
    plc = &_plc;
}

void plcControlThread::run(){
    //QTimer::singleShot(0, this, SLOT(terminateThis()));

    if (!stopped){

        if (commandState == _CMD_CHECK){            // reset cmds
            check();
        } else
            if (plc->plcInteract) {         // if plc is successfully connected

                if (commandState == _CMD_CENTER){           // action to center
                    result = plcCmdCenter();                             // no action
                    result = result & plcCmdStopReset();                          // reset stop cmd in case of existance
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_RIGHT){            // action to right
                    result = plcCmdRight();                              // right
                    result = result & plcCmdStopReset();                          // reset stop cmd in case of existance
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_LEFT){             // action to left
                    result = plcCmdLeft();                               // left
                    result = result & plcCmdStopReset();                          // reset stop cmd in case of existance
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_STOP){             // stop
                    result = plcCmdCenter();                             // no action
                    result = result & plcCmdStop();                               // stop
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_EMERGENCY_ACT){    // emergency active
                    result = plcCmdCenter();                             // no action
                    result = result & plcCmdStop();                               // stop
                    result = result & plcCmdEmergencyAct();                       // Emergency active
                } else
                if (commandState == _CMD_EMERGENCY_PSV){    // emergency passive
                    result = plcCmdStopReset();                          // reset stop cmd in case of existance
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_RESET){            // reset cmds
                    result = plcCmdCenter();                             // no action
                    result = result & plcCmdStopReset();                          // reset stop cmd in case of existance
                    result = result & plcCmdEmergencyPsv();                       // Emergency passive
                } else {
                    // state unknown, behave as emergency
                    result = plcCmdCenter();             // no action
                    result = result & plcCmdStop();               // stop
                    result = result & plcCmdEmergencyAct();       // Emergency active
                }
            }
    }
    //for(;;);
    stopped = false;
    //exec();
}

void plcControlThread::stop(){
    stopped = true;
}

bool plcControlThread::plcCmdCenter(){
    int result1 = plc->clrBit(dbNo, right_BYTE, right_BIT);
    int result2 = plc->clrBit(dbNo, left_BYTE, left_BIT);

    if (result1 == 0 && result2 == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdRight(){
    int result1 = plc->clrBit(dbNo, left_BYTE, left_BIT);
    int result2 = plc->setBit(dbNo, right_BYTE, right_BIT);

    if (result1 == 0 && result2 == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdLeft(){
    int result1 = plc->clrBit(dbNo, right_BYTE, right_BIT);
    int result2 = plc->setBit(dbNo, left_BYTE, left_BIT);

    if (result1 == 0 && result2 == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdStop(){
    int result = plc->setBit(dbNo, stop_BYTE, stop_BIT);

    if (result == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdStopReset(){
    int result = plc->clrBit(dbNo, stop_BYTE, stop_BIT);

    if (result == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdEmergencyAct(){
    int result = plc->setBit(dbNo, emergency_BYTE, emergency_BIT);

    if (result == 0)
        return true;
    else
        return false;
}

bool plcControlThread::plcCmdEmergencyPsv(){
    int result = plc->clrBit(dbNo, emergency_BYTE, emergency_BIT);

    if (result == 0)
        return true;
    else
        return false;
}

void plcControlThread::check(){
    plcInteract = plc->plcInteract;

    // check plc existance
    if (plcInteract){
        checkResult = plc->readBits(dbNo, right_BYTE, right_BIT);      // test read

        // if not read
        if (checkResult < 0){
            plc->connect(plcUrl.toUtf8());   // try re-connect
        } else {
            //
        }
     } else {
        // check if connection to plc is not established
        plc->connect(plcUrl.toUtf8());       // try re-connect
     }

}

void plcControlThread::disconnect(){
    plc->disconnect();
}

void plcControlThread::terminateThis(){
    //plc->plcInteract = true;
    //QThread::quit();
    //this->terminate();
    //qDebug() << "Timer event!!";
}

plcControlThread::~plcControlThread(){
    delete plc;
}
