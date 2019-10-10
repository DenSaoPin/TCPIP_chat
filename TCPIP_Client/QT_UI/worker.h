#pragma once
#ifndef WORKER_H
#define WORKER_H

#include <QObject>

 class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

public slots:
    virtual void doWork() = 0;
signals:
    void resultReady();
};

#endif // WORKER_H

