#pragma once
#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "worker.h"

class ThreadController : public QObject
{
    Q_OBJECT
    QThread* m_pWorkerThread;
    Worker* m_pWorker;

public:
    explicit ThreadController(Worker *pWorker, QObject *parent = nullptr);

    ~ThreadController()
    {
        m_pWorkerThread->quit();
        m_pWorkerThread->wait();
    }
public slots:
    void handleResults();
signals:
    void operate();
};

#endif // THREADCONTROLLER_H
