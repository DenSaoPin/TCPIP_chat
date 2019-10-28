#include "threadcontroller.h"

ThreadController::ThreadController(Worker *pWorker, QObject *parent) : QObject(parent)
{
        m_pWorkerThread = new QThread(this);
        m_pWorker = pWorker;
        m_pWorker->moveToThread(m_pWorkerThread);
        this->moveToThread(m_pWorkerThread );

        connect(this, &ThreadController::operate, m_pWorker, &Worker::doWork);

        m_pWorkerThread->start();

        //connect(m_pWorker, &Worker::resultReady, this, &ThreadController::handleResults);

        emit operate();
}

void ThreadController::handleResults()
{

}
