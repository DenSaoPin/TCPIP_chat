#ifndef CLIENTSETTINGS_H
#define CLIENTSETTINGS_H

#include "QString"

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

private:
    QString m_name;
    QString m_adress;
    QString m_port;
};

#endif // CLIENTSETTINGS_H
