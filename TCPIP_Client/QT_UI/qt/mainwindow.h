#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"
#include "QString"

class Client;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Client *pClient, QWidget *parent = 0);
    static void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage);
    static MainWindow* m_pI;
    //static void CallbackFunk(const char *szName, const int* messageType, const char *szMessage);

    ~MainWindow();

public slots:
    void PrintMessage(QString& str);

signals:

private slots:
    void on_button_Send_clicked();

    void on_actionReconnect_triggered();

private:
    Ui::MainWindow *ui;
    Client *pClient;
};

#endif // MAINWINDOW_H
