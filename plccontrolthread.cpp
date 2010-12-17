#include "plccontrolthread.h"

#include "mainwindow.h"

extern MainWindow *w;

plcControlThread::plcControlThread(s7 &_plc){
    stopped = false;
    plc = &_plc;
}

void plcControlThread::run(){
    if (!stopped){
        if (commandState == _CMD_CHECK){            // reset cmds
            check();
        } else

            if (plc->plcInteract) {         // if plc is successfully connected

                if (commandState == _CMD_CENTER){           // action to center
                    plcCmdCenter();                             // no action
                    plcCmdStopReset();                          // reset stop cmd in case of existance
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_RIGHT){            // action to right
                    plcCmdRight();                              // right
                    plcCmdStopReset();                          // reset stop cmd in case of existance
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_LEFT){             // action to left
                    plcCmdLeft();                               // left
                    plcCmdStopReset();                          // reset stop cmd in case of existance
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_STOP){             // stop
                    plcCmdCenter();                             // no action
                    plcCmdStop();                               // stop
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_EMERGENCY_ACT){    // emergency active
                    plcCmdCenter();                             // no action
                    plcCmdStop();                               // stop
                    plcCmdEmergencyAct();                       // Emergency active
                } else
                if (commandState == _CMD_EMERGENCY_PSV){    // emergency passive
                    plcCmdStopReset();                          // reset stop cmd in case of existance
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else
                if (commandState == _CMD_RESET){            // reset cmds
                    plcCmdCenter();                             // no action
                    plcCmdStopReset();                          // reset stop cmd in case of existance
                    plcCmdEmergencyPsv();                       // Emergency passive
                } else {
                    // state unknown, behave as emergency
                    plcCmdCenter();             // no action
                    plcCmdStop();               // stop
                    plcCmdEmergencyAct();       // Emergency active
                }
            }
    }

    stopped = false;
}

void plcControlThread::stop(){
    stopped = true;
}

int plcControlThread::plcCmdCenter(){
    plc->clrBit(w->DB_NO, w->left_VMEM_BYTE, w->left_BITofBYTE);
    int result = plc->clrBit(w->DB_NO, w->right_VMEM_BYTE, w->right_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdRight(){
    plc->clrBit(w->DB_NO, w->left_VMEM_BYTE, w->left_BITofBYTE);
    int result = plc->setBit(w->DB_NO, w->right_VMEM_BYTE, w->right_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdLeft(){
    plc->clrBit(w->DB_NO, w->right_VMEM_BYTE, w->right_BITofBYTE);
    int result = plc->setBit(w->DB_NO, w->left_VMEM_BYTE, w->left_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdStop(){
    int result = plc->setBit(w->DB_NO, w->stop_VMEM_BYTE, w->stop_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdStopReset(){
    int result = plc->clrBit(w->DB_NO, w->stop_VMEM_BYTE, w->stop_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdEmergencyAct(){
    int result = plc->setBit(w->DB_NO, w->emergency_VMEM_BYTE, w->emergency_BITofBYTE);
    return result;
}

int plcControlThread::plcCmdEmergencyPsv(){
    int result = plc->clrBit(w->DB_NO, w->emergency_VMEM_BYTE, w->emergency_BITofBYTE);
    return result;
}

void plcControlThread::check(){
    plcInteract = plc->plcInteract;

    // check plc existance
    if (plcInteract){
        result = plc->readBits(w->DB_NO, w->right_VMEM_BYTE, w->right_BITofBYTE);      // test read
        //result = plc->readBits(1, 0, 1);      // test read

        // if not read
        if (result < 0){
            plc->connect(w->urlPLC.toString().toUtf8());   // try re-connect
         } else {
         }
     } else {
        // check if connection to plc is not established
        plc->connect(w->urlPLC.toString().toUtf8());       // try re-connect
     }

}
