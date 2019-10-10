#pragma once
#ifndef MAINLOOPWORKER_H
#define MAINLOOPWORKER_H

#include "worker.h"

class MainLoopWorker : public Worker
{
public:
    MainLoopWorker();
public slots:
    void doWork() override;
signals:
    void resultReady();
};

#endif // MAINLOOPWORKER_H
