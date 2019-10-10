#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "ChatClientAPI.h"
#include "mainwindow.h"
#include "clientsettings.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(MainWindow *window, QObject *parent = nullptr);

public:
    MainWindow* pWindow;
    EClientStatus DllStatus;
    ClientSettings* pSettings;

signals:

public slots:
};

#endif // CLIENT_H
