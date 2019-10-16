#include "clientsettings.h"
#include "../../TCPIP_CLIENT_DLL/public/ChatClientAPI.h"
#include "client.h"

ClientSettings::ClientSettings()
{
}

QString ClientSettings::GetName()
{
    return m_name;
}
QString ClientSettings::GetAdress()
{
    return m_adress;
}
QString ClientSettings::GetPort()
{
    return m_port;
}

void ClientSettings::SetName(QString name)
{
    m_name = name;
}
void ClientSettings::SetAdress(QString adress)
{
    m_adress = adress;
}
void ClientSettings::SetPort(QString port)
{
    m_port = port;
}

void ClientSettings::SetupToDll()
{
    QByteArray bName = m_name.toLocal8Bit();
    const char *sz_name = bName.data();

    QByteArray bAdress = m_adress.toLocal8Bit();
    const char *sz_adress = bAdress.data();

    QByteArray bPort = m_port.toLocal8Bit();
    const char *sz_port = bPort.data();

    SetConnectionParams(sz_name, sz_adress, sz_port);
}

