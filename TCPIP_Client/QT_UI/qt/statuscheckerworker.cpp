#include "statuscheckerworker.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"
#include "mainwindow.h"
#include <QThread>
StatusCheckerWorker::StatusCheckerWorker()
{

}

void StatusCheckerWorker::doWork()
{
    EClientStatus status = eInvalid;
    while(status != eShutDown)
    {
        status = GetClientDllStatus();
        QThread::msleep(500);

        //emit resultReady();
    }
}
