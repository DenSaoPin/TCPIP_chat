#pragma once
#ifndef STATUSCHECKERWORKER_H
#define STATUSCHECKERWORKER_H

#include "worker.h"
#include "public/ChatClientAPI.h"

class StatusCheckerWorker : public Worker
{
    Q_OBJECT
public:
    StatusCheckerWorker();
public slots:
    void doWork() override;
signals:
    void resultReady();
    void statusUpdate(EClientStatus status);
};

#endif // STATUSCHECKERWORKER_H
