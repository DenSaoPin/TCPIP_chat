#include "threadcontroller.h"
#include "client.h"
#include "mainwindow.h"
#include "clientsettings.h"
#include "mainloopworker.h"
#include "statuscheckerworker.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"
#include <QStatusBar>
#include <QMetaType>

Client::Client(ClientSettings* pSettings, QObject *parent) : QObject(parent)
{   
    DllStatus = eInvalid;
    this->pSettings = pSettings;

    QStatusBar* pSB = Window.statusBar();
    pSB->showMessage("Not Updated");


    m_pWorker = new MainLoopWorker();
    pMainLoopController = new ThreadController(m_pWorker);

    m_pStWorker = new StatusCheckerWorker();
    pStatusCheckerController = new ThreadController(m_pStWorker);
    connect(m_pStWorker, &StatusCheckerWorker::statusUpdate, this, &Client::ClientStatusUpdate);
}

Client::~Client()
{
    delete pMainLoopController;
    delete pStatusCheckerController;
    delete m_pWorker;
    delete m_pStWorker;
}

void Client::ClientStatusUpdate(EClientStatus status)
{
    if(DllStatus != status)
    {
        DllStatus = status;
         QStatusBar* pSb = Window.statusBar();
         QString tStatus;
         switch(status)
         {
         case eInvalid:
             tStatus = "Invalid";
             break;
         case eStartWSA:
             tStatus = "StartWSA";
             break;
         case eInitializeSocket:
             tStatus = "InitializeSocket";
             break;
         case eIdle:
             tStatus = "Idle";
             break;
         case eTerminating:
             tStatus = "Terminating";
             break;
         case eShutDown:
             tStatus = "ShutDown";
             break;
         }
        pSb->showMessage(tStatus);
    }
}
