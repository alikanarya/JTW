#ifndef IMGPROCESSTHREAD_H
#define IMGPROCESSTHREAD_H

#include <QThread>

class imgProcessThread : public QThread {

    Q_OBJECT

public:

    explicit imgProcessThread();
     ~imgProcessThread();

    void run();
    void stop();

signals:

private:

};

#endif // IMGPROCESSTHREAD_H
