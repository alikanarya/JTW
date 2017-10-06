#ifndef AUTOFOCUSTHREAD_H
#define AUTOFOCUSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class autoFocusThread : public QThread {

    Q_OBJECT

public:

    autoFocusThread(int _initialSampleSize, int _iterNumber);
    void stop();

    QWaitCondition condition;
    bool restart = true;
    int initialSampleSize;
    int sampleSize;
    int iterNumber;
    float step;

protected:
    virtual void run();

private:

    bool mStop;
    QMutex mMutex;

};

#endif // AUTOFOCUSTHREAD_H
