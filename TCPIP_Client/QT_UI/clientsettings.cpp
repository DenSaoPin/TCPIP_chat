#include "clientsettings.h"

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

