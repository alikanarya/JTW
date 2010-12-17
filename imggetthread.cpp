#include "imggetthread.h"

#include "mainwindow.h"

extern MainWindow *w;

imgGetThread::imgGetThread(){
}

void imgGetThread::run(){
    if (w->play && !w->pause){
        w->imageGetter->run();      //request image from cam
    }
}
