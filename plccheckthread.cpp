#include "plccheckthread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

plcCheckThread::plcCheckThread(s7 &_plc){
    stopped = false;
    plc = &_plc;

}

void plcCheckThread::run(){
    if (!stopped){
        plcInteract = plc->plcInteract;

        // check plc existance
        if (plcInteract){
            result = plc->readBits(1, 0, 1);      // test read

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

    stopped = false;
}

void plcCheckThread::stop(){
    stopped = true;
}

//plcCheckThread::~plcCheckThread(){}
