#ifndef IMGGETTHREAD_H
#define IMGGETTHREAD_H

#include <QThread>

class imgGetThread : public QThread{
    Q_OBJECT

public:
    imgGetThread();
    void run();
};

#endif // IMGGETTHREAD_H
