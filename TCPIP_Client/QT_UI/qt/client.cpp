#include "client.h"
#include "mainwindow.h"
#include "clientsettings.h"
#include "mainloopworker.h"
#include "statuscheckerworker.h"
#include "threadcontroller.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"

Client::Client(MainWindow *window, ClientSettings* pSettings, QObject *parent) : QObject(parent)
{   
    pWindow = window;
    DllStatus = eInvalid;
    this->pSettings = pSettings;

    m_pWorker = new MainLoopWorker();
    pMainLoopController = new ThreadController(m_pWorker);

    m_pStWorker = new StatusCheckerWorker();
    pStatusCheckerController = new ThreadController(m_pStWorker);
}

Client::~Client()
{
    delete pMainLoopController;
    delete pStatusCheckerController;
    delete m_pWorker;
    delete m_pStWorker;
}
