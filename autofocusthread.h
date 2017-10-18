#ifndef AUTOFOCUSTHREAD_H
#define AUTOFOCUSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class autoFocusThread : public QThread {

    Q_OBJECT

public:

    autoFocusThread(double _start, double _end, int _initialSampleSize, int _iterNumber);
    void stop();

    QWaitCondition condition;
    bool restart = true;
    bool stopCmd = false;
    int initialSampleSize;
    int sampleSize;
    int iterNumber;
    double dMax;
    double step;
    double sampleStart, sampleEnd;
    double offset;
    int i, j;
    double pos;

protected:
    virtual void run();

private:

    bool mStop;
    QMutex mMutex;

signals:
    void setFocusPos(float);
    void iterationFinished();

};

#endif // AUTOFOCUSTHREAD_H
