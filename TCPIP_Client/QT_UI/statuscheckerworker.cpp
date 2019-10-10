#include "statuscheckerworker.h"
#include <ChatClientAPI.h>
#include "mainwindow.h"
#include <QThread>
StatusCheckerWorker::StatusCheckerWorker()
{

}

void StatusCheckerWorker::doWork()
{
    EClientStatus status;
    while(status != eShutDown)
    {
        status = GetClientDllStatus();
        QThread::msleep(500);

        //emit resultReady();
    }
}
