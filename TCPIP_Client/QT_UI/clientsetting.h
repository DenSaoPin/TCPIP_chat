#ifndef CLIENTSETTING_H
#define CLIENTSETTING_H

#include <QObject>

class ClientSetting : public QObject
{
    Q_OBJECT
public:
    explicit ClientSetting(QObject *parent = nullptr);

    QString Name;
    QString Adress;
    QString Port;

signals:

public slots:
};

#endif // CLIENTSETTING_H
