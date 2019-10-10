#pragma once
#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include <QString>
//#include "client.h"
class Client;
class ClientSettings
{

public:
    ClientSettings();

    QString GetName();
    QString GetAdress();
    QString GetPort();

    void SetName(QString);
    void SetAdress(QString);
    void SetPort(QString);

    void SetupToDll();

private:
    QString m_name;
    QString m_adress;
    QString m_port;
};

#endif // CLIENTSETTINGS_H
