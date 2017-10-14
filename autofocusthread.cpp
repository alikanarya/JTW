#include "autofocusthread.h"

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

autoFocusThread::autoFocusThread(double _start, double _end, int _initialSampleSize, int _iterNumber){
    sampleStart = _start;
    sampleEnd = _end;
    initialSampleSize = _initialSampleSize;
    iterNumber = _iterNumber;
}

void autoFocusThread::run(){

    //qDebug() << "thread started";

    sampleSize = initialSampleSize;
    dMax = 1.0;

    double dMaxPrev = 1;
    double stepPrev = 1;

    for (i=0; i<iterNumber; i++) {

        dMax = sampleEnd - sampleStart;
        if (i==0){
            step = dMax / (sampleSize-1);
        } else {
            step = dMax * stepPrev / dMaxPrev;
            sampleSize = dMax / step;
        }
        step = round(step*1000000) / 1000000.0;

        j=0;
        for (pos=sampleStart; pos<=sampleEnd; pos+=step) {
            pos = round(pos*1000000)/1000000.0;
            emit setFocusPos(pos);
            mMutex.lock();
            condition.wait(&mMutex);
            mMutex.unlock();
            j++;
        }

        stepPrev = step;
        dMaxPrev = dMax;

        emit iterationFinished();

        mMutex.lock();
        condition.wait(&mMutex);
        mMutex.unlock();
    }
}



/*
while (!mStop) {
    for (int i=0; i<5; i++){
        QThread::msleep(1000);
        qDebug() << i << " ,  " << restart;
        mMutex.lock();
        //if (restart)
            condition.wait(&mMutex);
        mMutex.unlock();
    }
}
*/
