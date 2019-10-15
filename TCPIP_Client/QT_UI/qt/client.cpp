#include "client.h"
#include "mainwindow.h"
#include "clientsettings.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"

Client::Client(MainWindow *window, QObject *parent) : QObject(parent)
{   
    pWindow = window;
    DllStatus = eInvalid;
    pSettings = new ClientSettings();
}
