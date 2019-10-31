#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "mainwindow.h"
#include "clientsettings.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"
#include <QMetaType>

class ThreadController;
class MainLoopWorker;
class StatusCheckerWorker;

class Client : public QObject
{
    Q_OBJECT

    MainLoopWorker *m_pWorker;
    StatusCheckerWorker *m_pStWorker;
public:
    explicit Client(ClientSettings* pSettings, QObject *parent = nullptr);

    virtual ~Client();

public:
    MainWindow Window{this};
    ThreadSafe<EClientStatus> DllStatus{eInvalid};
    ClientSettings* pSettings;

    ThreadController* pMainLoopController;
    ThreadController* pStatusCheckerController;

signals:

public slots:
    void ClientStatusUpdate(EClientStatus status);
};

#endif // CLIENT_H
