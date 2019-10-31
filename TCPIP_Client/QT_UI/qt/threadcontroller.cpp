#include "threadcontroller.h"
#include <QDebug>

#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QString>

class MyThread : public QThread
{
public:
    // constructor
    // set name using initializer
    ~MyThread() override
    {
        printf("Destruct");
        qDebug("Destruct");
    }
private:
};

#endif // MYTHREAD_H


ThreadController::ThreadController(Worker *pWorker, QObject *parent) : QObject(parent)
{
        m_pWorkerThread = new QThread();
        m_pWorker = pWorker;
        m_pWorker->moveToThread(m_pWorkerThread);
        this->moveToThread(m_pWorkerThread );

        connect(m_pWorkerThread, &QThread::started,this, &ThreadController::PrintStart );
        connect(m_pWorkerThread, &QThread::finished,this, &ThreadController::PrintFinish );
        connect(m_pWorkerThread, &QThread::destroyed,this, &ThreadController::PrintDestroy );

        connect(this, &ThreadController::operate, m_pWorker, &Worker::doWork);
        m_pWorkerThread->start();
}

void ThreadController::Operate()
{
    emit operate();
}
void ThreadController::Kill()
{
    m_pWorkerThread->quit();
    m_pWorkerThread->wait(100);
    m_pWorkerThread->terminate();
}



void ThreadController::handleResults()
{
}

void ThreadController::PrintStart()
{
    qDebug("thread started");
}
void ThreadController::PrintFinish()
{
    qDebug("thread finished");
}
void ThreadController::PrintDestroy()
{
    qDebug("thread destroyed");
}

ThreadController::~ThreadController()
{
    m_pWorkerThread->quit();
    m_pWorkerThread->wait(100);
    m_pWorkerThread->terminate();
}
