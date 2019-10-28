#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "mainwindow.h"
#include "clientsettings.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"

class ThreadController;
class MainLoopWorker;
class StatusCheckerWorker;

class Client : public QObject
{
    Q_OBJECT

    MainLoopWorker *m_pWorker;
    StatusCheckerWorker *m_pStWorker;
public:
    explicit Client(MainWindow *window, ClientSettings* pSettings, QObject *parent = nullptr);

    virtual ~Client();

public:
    MainWindow* pWindow;
    ThreadSafe<EClientStatus> DllStatus{eInvalid};
    ClientSettings* pSettings;

    ThreadController* pMainLoopController;
    ThreadController* pStatusCheckerController;

signals:

public slots:
};

#endif // CLIENT_H
