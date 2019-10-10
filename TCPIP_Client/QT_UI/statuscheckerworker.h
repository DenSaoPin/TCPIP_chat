#pragma once
#ifndef STATUSCHECKERWORKER_H
#define STATUSCHECKERWORKER_H

#include "worker.h"

class StatusCheckerWorker : public Worker
{
public:
    StatusCheckerWorker();
public slots:
    void doWork() override;
signals:
    void resultReady();
};

#endif // STATUSCHECKERWORKER_H
