#ifndef AUTOFOCUSTHREAD_H
#define AUTOFOCUSTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class autoFocusThread : public QThread {

    Q_OBJECT

public:

    autoFocusThread();
    void stop();

    QWaitCondition condition;
    bool restart;

protected:
    virtual void run();

private:

    bool mStop;
    QMutex mMutex;

};

#endif // AUTOFOCUSTHREAD_H
