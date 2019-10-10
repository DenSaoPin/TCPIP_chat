#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnRecieveMessage(const char *szName, const int* messageType, const char *szMessage)
{
    QString name (szName);
    QString message (szMessage);

    ui->textBox_Out->append(name + ": " + message + "\n");
}
