#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"

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

private slots:

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
