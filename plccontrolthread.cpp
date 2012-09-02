#include "plccontrolthread.h"

#include "mainwindow.h"

extern MainWindow *w;

plcControlThread::plcControlThread(int type, QString url){
    plcType = type;
    plcUrl = url;
    plc = new s7();

    plc->changeType(plcType);
    plcType = plc->plcType;  // to fix in case of wrong selection

    if (plcType == 0) dbNo = 1;    // for S7-200

    stopped = false;
    result = false;

    readLength = 1;
    readBuffer = new unsigned char[readLength];

    // z control additions
    readBufferInt = new unsigned char[2];
    //for (int i = 0; i < 4; i++) readBufferInt[i] = 0;

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

            if (w->goX) {
                if (commandState == _CMD_CENTER){           // no action
                    result = plcCmdCenter();
                } else
                if (commandState == _CMD_RIGHT){            // action to right
                    result = plcCmdRight();
                } else
                if (commandState == _CMD_LEFT){             // action to left
                    result = plcCmdLeft();
                } else
                if (commandState == _CMD_STOP){             // stop
                    result = plcCmdStop();
                } else
                if (commandState == _CMD_EMERGENCY_ACT){    // emergency active
                    result = plcCmdEmergencyAct();
                } else
                if (commandState == _CMD_EMERGENCY_PSV){    // emergency passive
                    result = plcCmdEmergencyPsv();
                } else
                if (commandState == _CMD_RESET){            // reset cmds
                    result = plcCmdCenter();                             // no action
                } else {
                    // state unknown, behave as emergency
                    result = plcCmdEmergencyAct();                       // Emergency active
                }
            }

            if (commandRead) {

                readPLC();
                //readDistanceValue();
                commandRead = false;
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
    unsigned char buffer = MASK_CMD_CENTER;

    int result = plc->writeBytes(dbNo, byteNo, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmdRight(){
    /*
    int result1 = plc->clrBit(dbNo, left_BYTE, left_BIT);
    int result2 = plc->setBit(dbNo, right_BYTE, right_BIT);
    */
    unsigned char buffer = MASK_CMD_RIGHT;

    int result = plc->writeBytes(dbNo, byteNo, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmdLeft(){
    /*
    int result1 = plc->clrBit(dbNo, right_BYTE, right_BIT);
    int result2 = plc->setBit(dbNo, left_BYTE, left_BIT);
    */
    unsigned char buffer = MASK_CMD_LEFT;

    int result = plc->writeBytes(dbNo, byteNo, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmdStop(){
    unsigned char buffer = MASK_CMD_STOP;

    int result = plc->writeBytes(dbNo, byteNo, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmdStopReset(){
    int result = plc->clrBit(dbNo, byteNo, 2);

    return (result == 0);
}

bool plcControlThread::plcCmdEmergencyAct(){
    unsigned char buffer = MASK_CMD_EMG;

    int result = plc->writeBytes(dbNo, byteNo, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmdEmergencyPsv(){
    int result = plc->clrBit(dbNo, byteNo, 3);

    return (result == 0);
}


bool plcControlThread::plcCmd_Z_Center(){
    unsigned char buffer = MASK_CMD_Z_CENTER;

    int result = plc->writeBytes(dbNo, byteNo2, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmd_Z_Up(){
    unsigned char buffer = MASK_CMD_Z_UP;

    int result = plc->writeBytes(dbNo, byteNo2, 1, &buffer);

    return (result == 0);
}

bool plcControlThread::plcCmd_Z_Down(){
    unsigned char buffer = MASK_CMD_Z_DOWN;

    int result = plc->writeBytes(dbNo, byteNo2, 1, &buffer);

    return (result == 0);
}


bool plcControlThread::readPLC(){

    int result = plc->readBytes(dbNoRead, 0, readLength, readBuffer);

    if (result == 0){

        unsigned char *byteptr = readBuffer;
        unsigned char byte = *byteptr;

        bool value;

        // read machine state
        int bitValue = w->machineNo - 1;
        if (getBitofByte(byte, bitValue) == 1) value = true; else value = false;
        w->mak_aktif_now = value;


        return true;
    } else {
        return false;
    }
}

bool plcControlThread::readDistanceValue(){
// customized for s7-200 start address:VW2
// customized for s7-300 start address:DBX.dbw2

    int result;

    result = plc->readBytes(dbNoRead, 2, 2, readBufferInt);

    if (result == 0){

        distanceRaw = plc->getS16(readBufferInt);
        w->distanceRaw = distanceRaw;

    } else {
        return false;
    }

    return true;
}


int plcControlThread::getBitofByte(unsigned char byte, int bitNo){

    return (byte >> bitNo) & 0x01;
}


void plcControlThread::check(){
    plcInteract = plc->plcInteract;

    // check plc existance
    if (plcInteract){
        checkResult = plc->readBits(dbNo, byteNo, 0);      // test read

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
