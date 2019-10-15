#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"
#include "QString"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage);
    ~MainWindow();

public slots:
    void PrintMessage(QString& str);

signals:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
