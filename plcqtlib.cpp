#include "plcqtlib.h"
#include <QTcpSocket>
#include <nodave.h>

PlcQtLib::PlcQtLib(QString _ip,int _dbNr, int _dbSize,unsigned char*_buf):QThread(){
   ip=_ip;
   dbNr=_dbNr;
   dbSize=_dbSize;
   dbBlock=_buf;
}

PlcQtLib::~PlcQtLib()
{
    _stopThread=true;
}

//funkcje interfejsu rejestrujące wątki
void PlcQtLib::writeByte(int dbb, int data)
{
    QMutexLocker m(&mutex);
    kolejkaZadan.enqueue(std::bind(&PlcQtLib::_writeByte,this,dbb,data));
}

void PlcQtLib::writeWord(int dbw, int data)
{
    QMutexLocker m(&mutex);
    kolejkaZadan.enqueue(std::bind(&PlcQtLib::_writeWord,this,dbw,data));
}

void PlcQtLib::writeDword(int dbd, int data)
{
    QMutexLocker m(&mutex);
    kolejkaZadan.enqueue(std::bind(&PlcQtLib::_writeDword,this,dbd,data));
}

void PlcQtLib::setBit(int dbx, int bitNr)
{
    QMutexLocker m(&mutex);
    kolejkaZadan.enqueue(std::bind(&PlcQtLib::_setBit,this,dbx,bitNr));
}

void PlcQtLib::clearBit(int dbx, int bitNr)
{
    QMutexLocker m(&mutex);
    kolejkaZadan.enqueue(std::bind(&PlcQtLib::_clearBit,this,dbx,bitNr));
}

void PlcQtLib::stopThread()
{
    _stopThread=true;
}

//funkcje wątku
void PlcQtLib::_writeByte(int dbb, int data){
   BYTE _data=data;
   daveWriteBytes(dc,daveDB,dbNr,dbb,1,&_data);

}

void PlcQtLib::_writeWord(int dbw, int data)
{
    WORD _data=daveSwapIed_16(data);
    daveWriteBytes(dc,daveDB,dbNr,dbw,2,&_data);
}

void PlcQtLib::_writeDword(int dbd, int data)
{
    DWORD _data=daveSwapIed_32(data);
    daveWriteBytes(dc,daveDB,dbNr,dbd,4,&_data);

}

void PlcQtLib::_setBit(int dbx, int bitNr)
{
    daveSetBit(dc,daveDB,dbNr,dbx,bitNr);
}

void PlcQtLib::_clearBit(int dbx, int bitNr)
{
    daveClrBit(dc,daveDB,dbNr,dbx,bitNr);
}
//funkcja run
void PlcQtLib::run()
{
    QTcpSocket socket;
    socket.connectToHost(ip,102);
    if(!socket.waitForConnected(socketWaitForConnectedTime)){
        socket.disconnect();
        emit connected(false);
        return;
    }
    _daveOSserialType fds;
    daveInterface *di;
    fds.wfd =(HANDLE)socket.socketDescriptor();
    fds.rfd=fds.wfd;
    bool okPlc=false;
    if (fds.rfd>0){
        di = daveNewInterface(fds, "IF1",0, protocol, daveSpeed187k);    // 123:s7200, 122:s7300
        daveSetTimeout(di,5000000);
        dc=daveNewConnection(di,2,0,2);
        if(daveConnectPLC(dc)==0)
            okPlc=true;
        else
            okPlc=false;
    }
    if (okPlc) emit connected(true);
    while(okPlc&&(!_stopThread)){
        while (!kolejkaZadan.empty()) {
            QMutexLocker m(&mutex);
            kolejkaZadan.dequeue()();
        }
        int error=daveReadBytes(dc,daveDB,dbNr,0,dbSize,dbBlock);
        okPlc=!error;
        if(okPlc)emit dataReady();
        QThread::msleep(loopSleepTime);
    }

    socket.disconnectFromHost();
//    socket.waitForDisconnected();

    daveDisconnectPLC(dc);
    daveFree(dc);
    emit connected(false);
    kolejkaZadan.clear();
    QThread::msleep(disconnectSleepTime);
}

int PlcBYTEtoInt(PlcBYTE data)
{
   return data;
}

int PlcWORDtoInt(PlcWORD data)
{
   return daveSwapIed_16(data);
}

int PlcDWORDtoInt(PlcDWORD data)
{
   return daveSwapIed_32(data);
}

