#include "threadcontroller.h"

ThreadController::ThreadController(Worker &worker, QObject *parent) : QObject(parent)
{
        m_pWorkerThread = new QThread(this);
        m_pWorker = &worker;
        m_pWorker->moveToThread(m_pWorkerThread);
        this->moveToThread(m_pWorkerThread );

        m_pWorkerThread->start();

        connect(this, &ThreadController::operate, m_pWorker, &Worker::doWork);
        //connect(m_pWorker, &Worker::resultReady, this, &ThreadController::handleResults);

        emit operate();
}

void ThreadController::handleResults()
{

}
